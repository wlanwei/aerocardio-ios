#include "DspEngine.h"
#include "math.h"
#include "BEAT.h"
#include "HRCheck.h"
#include "VFdetect.h"
#include "WAVEFilter.h"
#include "BDAC.h"
#include "ABEcgCheck.h"
#include "FIBDetect.h"
#include "PafDetect.h"


DspEngine::DspEngine(){
	input = queue<Ecg>();
	output = queue<Ecg>();

	outputM = queue<EcgMark>();

	flg_run_dsp = 1;
	thrd_dsp = thread(&DspEngine::run_dsp, this);
	thrd_dsp.detach();

	int theType = -1;  // initionlization
	int theSps = -1;
	long theStartT = -1;
	long theStopT = -1;
}

void DspEngine::run_dsp(){
	static int theType = -1;  // initionlization
	static int theSps = -1;
	static long theStartT = -1;
	static long theStopT = -1;
	static long startDeviceT = 0; //设备启动时间
	//static BaseFilter* baseFilter;
	static long rr[2] ={0};
	while(1){
		P_SLEEP(50); 
		if(flg_run_dsp < 0){
			//cout << "thread exiting flag = " << flg_run_dsp << "\n";
			P_SLEEP(5000); 
			return;
		}else{
			Ecg ecg = get_ecg_raw();
			//TODO: processing here
			//N.B.: Do not create new thread here for filtering
			/**********************业务逻辑*****************/
			if(theType == -1 && theSps ==-1&& theStartT == -1 && theStopT == -1){// 初始化
				theType = ecg.getType();
				theSps = ecg.getSps();
				theStartT = ecg.getTime_start();
				theStopT = ecg.getTime_stop(); 
				startDeviceT = ecg.getTime_start();// 设备启动时间备份
				// baseFilter= new BaseFilter[theType];
				// for(int m =0; m<theType; m++){
				WAVEFilter(0,1,0);
				//}
				ResetBDAC();
			}else if(theStopT != ecg.getTime_start() || theSps != ecg.getSps() || theType != ecg.getType()){//if discrete or ecg configuration changed, reset filters and ecg waves
				theType = ecg.getType();
				theSps = ecg.getSps();
				theStartT = ecg.getTime_start();
				theStopT = ecg.getTime_stop();
				startDeviceT = ecg.getTime_start(); //更新设备启动时间
				// baseFilter= new BaseFilter[theType];
				//for(int m =0; m<theType; m++){
				WAVEFilter(0,1,0);
				//}
				ResetBDAC();
			}else{
				theStartT = ecg.getTime_start();
				theStopT = ecg.getTime_stop();
			}
			Ecg* ecgs  = ecg.deplex(ecg,ECGLEN);
			int*dataF = new int[ECGLEN];
			BEAT beat;
			static int brFre =0;
			long rList;
			int beatType;
			int EcgDATA[3][25] ={0};
			if(theSps == 500){
				for(int m =0; m<theType; m++){
					//cout<<"这是第"<<m<<"组滤波后数据 "<<endl;
					for (int n =0 ; n<25; n++){
						EcgDATA[m][n] = WAVEFilter(ecgs[m].getData()[n],0,m);
						//cout<<EcgDATA[m][n]<<",";
						if(m==1 && theStartT+2*n - startDeviceT >2000){//屏蔽掉开机启动后两秒的数据分析
							if(VFdetect(EcgDATA[m][n])){
								//cout<<"室颤检出了"<<endl;
								put_ecgMark_dsp(EcgMark(theStartT+2*n - 1000,theStartT+2*n,2,5,25));
							}
							beat = BeatDetectAndClassify(EcgDATA[m][n], 0) ;							
							if(beat.getDelay() !=0){
								rList = theStartT + 2*n -beat.getDelay() +beat.getAdjustR();
								if(rList < startDeviceT+2000){
									//todo  屏蔽掉这段数据的值
								}else{
									switch(beat.getBeatType()){
										case 1:
											beatType = 0;
											break;//normal beat
										case 5:
											beatType = 1;
											put_ecgMark_dsp(EcgMark(rList - 2000,rList,2,5,2));
											// premature venricular contracions
											break;
										case 14:
											beatType = 3;
											put_ecgMark_dsp(EcgMark(rList - 2000,rList,2,3,0));
											// noise
											break;
										case 100:
											//heavy noise
											beatType = 4;
											put_ecgMark_dsp(EcgMark(rList - 2000,rList,2,3,1));
											break;
										default:
											beatType = 0; //unKnown beat 							 
									}
									EcgMark ecgMarkHR= HRCheck(rList); //心率检测
									if(ecgMarkHR.getStartTime() != -1){
										//cout<<endl;
										//cout<<"*********心率："<<ecgMarkHR.getValue()<<endl;
										put_ecgMark_dsp(ecgMarkHR);
										if(ecgMarkHR.getValue()<55){
											//cout<<"心率过慢："<<endl;
											put_ecgMark_dsp(EcgMark(ecgMarkHR.getStartTime(),ecgMarkHR.getStopTime(),2,1,11));//心率过慢
										}else if(ecgMarkHR.getValue()>105){
											// cout<<"心率过快："<<endl;
											put_ecgMark_dsp(EcgMark(ecgMarkHR.getStartTime(),ecgMarkHR.getStopTime(),2,1,10));//心率过快
										} 
										if(brFre == 3){
											put_ecgMark_dsp(EcgMark(ecgMarkHR.getStartTime()-1000,ecgMarkHR.getStopTime(),2,2,(int)(ecgMarkHR.getValue()*1.0f/4.5f+0.5)));//心率过快
											brFre = 0;
										}
										brFre ++;
									}
									EcgMark ecgMarkAB = ABEcgCheck(rList);//异常心率检测 
									if(ecgMarkAB.getStartTime()!=-1){
										put_ecgMark_dsp(ecgMarkAB);
									}

									if (beat.getAdjustR() != 0){
										rr[1] = rList;
										if(rr[0]!=0){
											beatType = PafDetect( (int)(rr[1]-rr[0]), (int)beatType,beat.getIsDomType());
											if(beatType == 2){
												//cout<<"有房早了"<<endl;
												put_ecgMark_dsp(EcgMark(rList - 5000,rList,2,4,3)); //Premature Atrial f 
											}
											EcgMark ecgMarkFIB  = FIBDetect((rr[1]-rr[0])*1.0f/1000, rList,(int)beatType );
											if(ecgMarkFIB.getStartTime()!=-1){
												put_ecgMark_dsp(ecgMarkFIB);
												//cout<<"有房颤了"<<endl;
											}
										}
										rr[0]= rr[1];
									}							   
								}
							}
						}
						dataF[m*25 + n] = EcgDATA[m][n];
					}	
				}
			}else{ // 采样率为1000时
				// 目前版本不考虑
			}
			Ecg ecgF(theType,theStartT,theStopT,theSps,dataF);  // After filter
			//for(int i=0; i<75;i++){
			//  cout<<ecgF.getData()[i]<<",";      //
			//}
			//Ecg ecgF = ecg.multiplex(ecgs,theType,ECGLEN);
			//put_ecg_dsp(ecgF);
			put_ecg_dsp(ecgF);
			//	cout << "thread working flag = " << flg_run_dsp << "\n";
			continue;
		}
	}
}

//blocking queue to the calling thread 
int DspEngine::put_ecg_raw(Ecg ecg){
	lock_guard<mutex> guard(mut_input);
	while(input.size() > DspEngine::MAX_QUEUE_IN){
		P_SLEEP(1000);
	}
	input.push(ecg);
	return 0;
}

//blocking queue to the calling thread
Ecg DspEngine::get_ecg_raw(){
	while(input.size() < 1){
		P_SLEEP(1000);
	}
	lock_guard<mutex> guard(mut_input);
	Ecg ecg = input.front();
	input.pop();
	return ecg;
}

//blocking queue to the thread calls it
int DspEngine::put_ecg_dsp(Ecg ecg){
    lock_guard<mutex> guard(mut_output);
	while(output.size() > DspEngine::MAX_QUEUE_OUT){
		P_SLEEP(1000);
	}
	output.push(ecg);
	return 0;
}

//blocking queue to the thread calls it
Ecg DspEngine::get_ecg_dsp(){
	while(output.size() < 1){
		P_SLEEP(1000);
	}
    lock_guard<mutex> guard(mut_output);
	Ecg ecg = output.front();
	output.pop();
	return ecg;
}


//blocking queue to the thread calls it
int DspEngine::put_ecgMark_dsp(EcgMark ecgMark){
	lock_guard<mutex> guard(mut_outputM);
	while(outputM.size() > DspEngine::MAX_QUEUE_OUT){
		P_SLEEP(1000);
	}
	outputM.push(ecgMark);
	return 0;
}

//blocking queue to the thread calls it
EcgMark DspEngine::get_ecgMark_dsp(){
	lock_guard<mutex> guard(mut_outputM);
	if(outputM.size() < 1){
		//P_SLEEP(1000);
		return EcgMark(-1,-1,-1,-1,-1);
	}else{
		EcgMark ecgMark = outputM.front();
		outputM.pop();
		return ecgMark;
	}
}



void DspEngine::stop_work(){
	flg_run_dsp = -1;
	//cout<<"setting flg to -1"<<"\n";
}

#include "Ecg.h"

Ecg::Ecg(){}
Ecg::Ecg(int initType, long initTime_start,long initTime_stop,int initSps,int*initData){
	type = initType;
	time_start = initTime_start;
	time_stop = initTime_stop;
	sps = initSps;
	if(type == 1){
		data = new int[25];
		memmove(data,initData,25*sizeof(int));
		//delete(initData);
	}
	else{
		data = new int[75];
		memmove(data,initData,75*sizeof(int));
		//delete(initData);
	}
}
Ecg::~Ecg(){}

/**
 * merge consecutive ecg into one ecg
 * @param ecgs
 * @return
 */
Ecg Ecg::merge(Ecg* ecgs ,int type,int len) {
	int *data = new int;
	int sizeData = 0;
	long startTime = -1;
	long stopTime = -1;
	//    int type = -1;
	for (int m = 0; m < type; m ++) {
		// sizeData += ecgs[m].data.length;
		if (m == 0) {
			startTime = ecgs[m].getTime_start();
			type = ecgs[m].getType();
			continue;
		}
		if (type != ecgs[m].getType()) {
			return Ecg();
		}
	}

	stopTime = ecgs[type - 1].getTime_stop();

	data = new int[sizeData];
	int startPos = 0;
	for (int m = 0; m < type; m ++) {
		for (int n =0; n<len/type; n++){
			data[m*len/type+n] = ecgs[m].getData()[n];
		}

		//System.arraycopy(ecgs[m].getData, 0, data, startPos, len);
		//startPos += ecgs[m].data.length;
	}

	return Ecg(ecgs[0].getType(), startTime, stopTime, ecgs[0].getSps(), data);
}
Ecg Ecg::multiplex(Ecg* ecgs,int type,int len) {
	bool isConsistent = true;
	long startTime = -1;
	long stopTime = -1;
	int sps = -1;
	int length = -1;
	for (int m = 0; m < type; m ++) {
		if (startTime < 0 || stopTime < 0 || sps < 0 || length < 0) {
			startTime = ecgs[m].getTime_start();
			stopTime = ecgs[m].getTime_stop();
			sps = ecgs[m].getSps();
			length = len;
		}
		else {
			if (startTime != ecgs[m].getTime_start() || stopTime != ecgs[m].getTime_stop() || sps != ecgs[m].getSps() || length != len ){
				isConsistent = false;
				break;
			}
		}
	}

	if (!isConsistent) {// abnormal 
		return Ecg();
	}
	else {
		int* data  = new int[len];
		for (int m = 0; m < type; m ++)
			for (int n = 0; n < len/type; n ++){
				data[m*len/type+n] = ecgs[m].getData()[n];
			}
		return Ecg(type, startTime, stopTime, sps, data);
	}
}
Ecg* Ecg::deplex(Ecg ecg,int len) {
	long starTime = ecg.getTime_start();
	long stopTime = ecg.getTime_stop();
	int sps = ecg.getSps();
	int type = ecg.getType();
	int* dataEcg = ecg.getData();

	Ecg* ecgs = new Ecg[type];   
	int **dataT= new int*[type];
	for (int n = 0; n < type; n ++) {
		dataT[n] = new  int[len/type];
	}

	for (int m = 0; m < len/type; m ++) {
		for (int n = 0; n < type; n ++) {
			dataT[n][m] = dataEcg[m*type+n];
		}
	}

	for (int n = 0; n < type; n ++) {
		ecgs[n] = Ecg(1, starTime, stopTime, sps, dataT[n]);
	}
	return ecgs;
}
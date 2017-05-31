/*****************************************************************************
FILE:  ABEcgCheck.cpp
AUTHOR:	wan 
CREATED:	2/16/2017
*******************************************************************************/
#include "ABEcgCheck.h"

EcgMark ABEcgCheck(long rLocation){//rlocation 是监测出的R点的位置，单位为毫秒
    static int counter =0;
	static long lastRLocation;
	static float  RRList[ABLEN] ={0};
	if (rLocation <= 0){
	    return EcgMark(-1, -1, -1, -1, -1);
	}else{
		if(counter == 0){
            lastRLocation = rLocation;
            return EcgMark(-1, -1, -1, -1, -1);
		}else if(counter <ABLEN-1){		
			RRList[counter] = round_AB(60/((rLocation - lastRLocation)/1000.0));// s
            lastRLocation = rLocation;
            counter++;
		    return EcgMark(-1, -1, -1, -1, -1);
		}else{
			RRList[counter] = round_AB(60/((rLocation - lastRLocation)/1000.0));
			lastRLocation = rLocation;
			
			vector <float> tempRList;
			//ArrayList<float> tempRList = new ArrayList<float>(counter);
			for (int m = 0; m <CHECK_LEN  ; m ++) {
			   tempRList.push_back(RRList[m]);
		    }
			int i,j,maxIndex,minIndex;
            float tempMax,tempMin,hrValue =0;
            for (i=0; i<2; i++){
                tempMax = *(tempRList.begin());
                tempMin = *(tempRList.begin());
                maxIndex = 0;
                minIndex = 0;
                for (j=1; j<counter-1-i*2; j++){
                    if(tempMax < *(tempRList.begin()+j)){
                        tempMax = *(tempRList.begin()+j);
                        maxIndex = j;
                    }
                    if(tempMin > *(tempRList.begin()+j)){
                        tempMin = *(tempRList.begin()+j);
                        minIndex = j;
                    }
                }
                if(maxIndex > minIndex){
                    tempRList.erase(tempRList.begin()+maxIndex);
                    tempRList.erase(tempRList.begin()+minIndex);
                }else if(maxIndex < minIndex){
                    tempRList.erase(tempRList.begin()+minIndex);
                    tempRList.erase(tempRList.begin()+maxIndex);
                }else{
                    if(maxIndex == 0){
                        tempRList.erase(tempRList.begin()+minIndex+1);
                        tempRList.erase(tempRList.begin()+maxIndex);
                    } else if(maxIndex == tempRList.size() -1){
                        tempRList.erase(tempRList.begin()+minIndex);
                        tempRList.erase(tempRList.begin()+maxIndex-1);
                    }else{
                        tempRList.erase(tempRList.begin()+minIndex);
                        tempRList.erase(tempRList.begin()+maxIndex-1);
                    }
                }
            }
			float meanV = 0;
			float result = 0;
			for (int m = 0; m <tempRList.size(); m ++) {
			    meanV+=tempRList[m];
		    }
			meanV/=tempRList.size();
			for (int m = 0; m < tempRList.size()  ; m ++) {
			    result+=(tempRList[m] - meanV)*(tempRList[m] - meanV);
		    }
			result/=tempRList.size();
			result = sqrt(result);
			meanV = 0;
			for (int m = 0; m < ABLEN  ; m ++) {
			    meanV+=60/RRList[m]*1000;
		    }
			long startTime = rLocation - (long)meanV;
			for (int m = 1; m < CHECK_LEN  ; m ++) {
			    RRList[m-1] = RRList[m];
		    }
			if (result >7.5) {
                return EcgMark(startTime, rLocation, 2, 5,12 );
            }
            else{
                return EcgMark(-1,-1,-1,-1,-1);
            }
		}
	} 
}
int round_AB( float val){
  return (val >0.0)?floor(val+0.5):ceil(val -0.5);
}
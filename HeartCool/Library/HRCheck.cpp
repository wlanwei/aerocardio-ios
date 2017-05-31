/*****************************************************************************
FILE:  HRCheck.cpp
AUTHOR:	wan 
CREATED:	2/16/2017
*******************************************************************************/
#include "HRCheck.h"

EcgMark HRCheck(long rLocation){//rlocation 是监测出的R点的位置，单位为毫秒
    static int counter =0;
	static long lastRLocation;
	static float  RRList[CHECK_LEN] ={0};
	if (rLocation <= 0){
	    return EcgMark(-1, -1, -1, -1, -1);
	}else{
		if(counter == 0){
            lastRLocation = rLocation;
			counter++;
            return EcgMark(-1, -1, -1, -1, -1);
		}else if(counter <CHECK_LEN){
			
			RRList[counter-1] = (rLocation - lastRLocation)/1000.0;// s
            lastRLocation = rLocation;
            counter++;
		    return EcgMark(-1, -1, -1, -1, -1);
		}else{
			RRList[counter-1] = (rLocation - lastRLocation)/1000.0;
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
			for (int m = 0; m < tempRList.size()  ; m ++) {
			    hrValue+=tempRList[m];
		    }
			hrValue/=tempRList.size();
			for (int m = 1; m < CHECK_LEN  ; m ++) {
			    RRList[m-1] = RRList[m];
		    }
			return EcgMark(rLocation-1000, rLocation, 2, 1, (int)(1.0f/hrValue*60));
		}
	} 
}

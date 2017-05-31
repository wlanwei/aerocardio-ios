/*****************************************************************************
FILE:  FIBDetect.cpp
AUTHOR:	wan 
CREATED:	2/16/2017
*******************************************************************************/
#include "FIBDetect.h"

EcgMark FIBDetect(float rr,long currentTime,int type){//
    static int counter =0;
	static int typeCounter =0;
	static float  TypeList[TYPELEN] ={0};
	static float  RRList[ANALYLEN] ={0};

	if(type == 2){
	   TypeList[typeCounter] = rr;
	   typeCounter++;
	   if(typeCounter == TYPELEN){
         float tempT = 0;
		 for (int m = 0; m<TYPELEN ; m++){
		     tempT += TypeList[m];
		 }
		 typeCounter =0;
	     return EcgMark((long)(currentTime - tempT*1000),currentTime,2,5,32);
	   }
	}
	if (counter < ANALYLEN-1){
		RRList[counter] = rr;
		counter++;
	    return EcgMark(-1, -1, -1, -1, -1);
	}else{
		RRList[counter] = rr;
		int i,j,maxIndex,minIndex;
		float tempMax,tempMin,meanSeg =0,rmssd=0;
		float tpr,entropy = 0,tprStats=0;
		float group[17] = {0};
		vector <float> tempRList;
			//ArrayList<float> tempRList = new ArrayList<float>(counter);
		for (int m = 0; m <ANALYLEN; m ++) {
			 tempRList.push_back(RRList[m]);
		}
		for (i=0; i<8; i++){
			tempMax = *(tempRList.begin());
            tempMin = *(tempRList.begin());
            maxIndex = 0;
            minIndex = 0;
			for (j=1; j<ANALYLEN-i*2; j++){
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
		   
		   for (int m=0; m<ANALYLEN ; m++){
			  meanSeg += RRList[m];
		   }
		   meanSeg /= ANALYLEN;

		   float difference[ANALYLEN -16-1] ={0};
		   for (int m=0; m<ANALYLEN -16-1 ; m++){
			  difference[m] = *(tempRList.begin()+m+1) - *(tempRList.begin()+m);
		   }

		   for (int m=0; m<ANALYLEN -16-1 ; m++){
			   rmssd += difference[m]*difference[m];
		   }
		   rmssd = sqrt(rmssd/meanSeg/ANALYLEN);
		   i=0;
		   for (int k=1; k<ANALYLEN-1; k++){
			    if((RRList[k]-RRList[k-1])*(RRList[k]-RRList[k+1])>0){
				    i=i+1;
			    }
		   }
		   tpr = i/(ANALYLEN-2);
		   // Distribution  using Shannon Entropy
		   float maxSeg = *(tempRList.begin());
		   float minSeg = *(tempRList.begin());
		   for (int m=0; m< ANALYLEN -16; m++){
		       if (*(tempRList.begin()+m)>maxSeg){
			      maxSeg = *(tempRList.begin()+m);
			   }
			   if (*(tempRList.begin()+m)<minSeg){
			       minSeg = *(tempRList.begin()+m);
			   }
		    }
            float step = (maxSeg -minSeg)/16;
            if(step !=0){
                for (i=0;i<ANALYLEN-16;i++){
        		   group[roundInt((*(tempRList.begin()+i)-minSeg)/step)]++; 
        	     }
        	float tempSum = 0;
			for(int m=0;m<17;m++){
			   tempSum+= group[m];
			}  
        	for (i=0; i<17; i++){
        		 group[i]/=tempSum;
        	}
        	   
        	for(i=0; i<16;i++){
        		if(group[i]>0){
        		 entropy += group[i]*log10(group[i]); 
        		}
        	}
         }
         entropy /=(-2.7726);
         tprStats = 0.5*(1+getErf(TPRMEAN,TPRSTD,tpr));

		 for (int m = 1; m < ANALYLEN; m ++) {
			RRList[m-1] = RRList[m];
		 }
		 if(rmssd >0.1f && tprStats<=0.9999 && entropy >0.7){
            long stopTime =currentTime;
			float tempT =0;
			for(int m=0; m<ANALYLEN; m++){
			    tempT += RRList[m];
			}
		    long startTime=currentTime - tempT*1000;
			return EcgMark(startTime,stopTime,2,5,32);
		 }else{
			   return EcgMark(-1,-1,-1,-1,-1);
		 }
	 }
}			

float getMax(float *data,int len){
	float x = data[0];
	for(int m=1; m<len ; m++){
		if (x<data[m]){
			x = data[m];
		}
	}
	return x;	
}

float getMin(float *data,int len){
	float x = data[0];
	for(int m=1; m<len ; m++){
		if (x>data[m]){
			 x = data[m];
		}
	}
	return x;	
}

int roundInt( float val){
  return (val >0.0)?floor(val+0.5):ceil(val -0.5);
}

/**** This is needed integral function :The function is : e^(-1*x^2)****/
float getErf(float mean, float std,float data){
		float outX=0;
		outX = (float) (2/sqrt(PI))*getIngeral(0,(float) ((data-mean)/std/sqrt(2)));
		return outX;	
}
/*** get Integral of function f*******/  
float getIngeral(float x0,float xn) { 
	int n=1000;
	float h = ((xn - x0) >0?(xn - x0):(x0 -xn)) / n; 
	float sum = 0;  
	for (float xi = 0; xi <= xn; xi = xi + h) { 
		sum += (float)(pow(E,-1 *xi*xi) + pow(E,-1*(xi + h)*(xi + h))) * h / 2; 
	}  
	return sum; 
} 

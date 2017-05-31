/*****************************************************************************
FILE:  VFdetect.cpp
AUTHOR:	wan 
CREATED:	2/16/2017
___________________________________________________________________________

VFdetect.cpp aim at deteting  ventricular fibrillation;
__________________________________________________________________________
 *******************************************************************************/
#include "VFdetect.h"

/******************************************************************************
 * Syntax:
 *	VFdetect(int datum,int init)
 * Description:
 *
 *	The filter buffers and static variables are reset if a value other than
 *	0 is passed to QRSFilter through init.
 *******************************************************************************/
bool VFdetect (double datum) {
	static double data[INTERTIME] = {0};
	static int counter =0;
	if(counter <INTERTIME){
		data[counter] = datum;
		counter++;
		return false;
	}
	else{
		double maxValue = getMax(data,INTERTIME);
		double minValue = getMin(data,INTERTIME);
		int binaryData[INTERTIME] = {0};
		if(abs(maxValue)>abs(minValue)){
			for (int i=0; i<INTERTIME; i++){
				if(data[i]>maxValue*0.2)
					binaryData[i] = 1;
			}
		}else{
			for (int i=0; i<INTERTIME; i++){
				if(data[i]< minValue*0.2)
					binaryData[i] = 1;
			}
		}
		int diffData[INTERTIME -1] = {0};
		for(int m =0; m<INTERTIME-1 ; m++){
			diffData[m] = binaryData[m+1] -binaryData[m];
		}

		int num = INTERTIME/4 -1;
		while (num > 0){
			if(diffData[num] == -1)
				break;
			num--;
		}
		float t1 = (INTERTIME/4-num)*1.0f*1000/STEP;
		int index =0;
		int indexEnd =0;
		num = INTERTIME/4 -1;
		for (int i=INTERTIME/4 -1; i<INTERTIME*3/4-1; i++){
			if (index ==0 && diffData[i]==1){
				num =i;
				index ++;
			}else{
				if(diffData[i]==1){
					index ++;
					indexEnd=i;
				}
			}
		}

		float t2 = (num - INTERTIME/4 + 1)*1.0f*1000/STEP;
		float t3 = (INTERTIME*3/4 - 1 - indexEnd)*1.0f*1000/STEP;

		num = INTERTIME*3/4 -1;
		while (num<INTERTIME-1){
			if(diffData[num] == 1)
				break;
			num++;
		}

		float t4 = (num - num*3/4 -1)*1.0f*1000/STEP;		
		float tci = 1000/(index-1 + t2/(t1+t2) +t3/(t3+t4));
		for (int m = STEP; m < INTERTIME; m ++) {
			data[m-STEP] = data[m];
			counter = INTERTIME - STEP;
		}
		if(tci<100 ){
			return true;
		}else{
			return false;
		}
	}
}
double getMax(double *data,int len){
	double x = data[0];
	for(int m=1; m<len ; m++){
		if (x<data[m]){
			x = data[m];
		}
	}

	return x;	
}
double getMin(double *data,int len){
	double x = data[0];
	for(int m=1; m<len ; m++){
		if (x>data[m]){
			x = data[m];
		}
	}
	return x;	
}

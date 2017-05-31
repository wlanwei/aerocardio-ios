/*****************************************************************************
FILE:  Ecg.h
AUTHOR:	uteamtec
Created:	2017/2/13
******************************************************************************/
#ifndef _ECG_	/* avoid multiple definitions */
#define _ECG_

#include <string.h>
//#define ECGLEN  75
static int ECGLEN =75;
class Ecg{
    public:
		Ecg();
		Ecg(int initType, long initTime_start,long initTime_stop,int initSps,int*initData);
		~Ecg();
		int getType(){return type;}
		long getTime_start(){return time_start;}
		long getTime_stop(){return time_stop;}
		int getSps(){return sps;}
		int* getData(){return data;}

		void setType(int initType){type = initType;}
		void setTime_start(long initTime_start){time_start = initTime_start;}
		void setTime_stop(long initTime_stop){time_stop = initTime_stop;}
		void setSps(int initSps){sps = initSps;}
		void setData(int *initData){
            /*ECGLEN = sizeof(initData)/sizeof(int);
            if(sizeof(data)/sizeof(int) != ECGLEN)
			   free(data);	
			int* data= (int*)malloc(sizeof(int)*ECGLEN);*/
			memmove(data,initData,sizeof(int)*ECGLEN);
		}

		 Ecg merge(Ecg* ecgs ,int type,int len);
		 Ecg multiplex(Ecg* ecgs,int type,int len);
		 Ecg* deplex(Ecg ecg,int len);

	private:
		int type;
		long time_start;
		long time_stop;
		int sps;
		int *data;

};
#endif

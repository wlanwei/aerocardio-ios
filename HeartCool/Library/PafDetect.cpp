/*****************************************************************************
FILE:  PafDetect.cpp
AUTHOR:	wan 
CREATED:	2/16/2017
*******************************************************************************/
#include "PafDetect.h"


static int sumnoise =0;
static int lastmean = 0;
static int lastwhetherDomType = 0;
static int  typeList[50] = {0};
static int id = 0;
static int lastrr = 0;
static int lastmeanrr = 0;
static int selectedrr[5] = {0};
int PafDetect(float rr,int type, int whetherDomType){//rlocation �Ǽ�����R���λ�ã���λΪ����
    
	int  meanrr;
	if(whetherDomType  == 1)
	    meanrr = stabmean(rr);
	else
		meanrr= lastmean;
	int apstats = type;

	if(type == 3|| type==4)
		sumnoise++;
	if(typeList[49] == 3 || typeList[49] == 4)
		sumnoise--;
	typeList[0] = type;	
		//  ��ǰ�Ĳ���ǰһ���Ĳ���Ϊ���������Ĳ�
	if( lastwhetherDomType == 1 && whetherDomType == 1 && typeList[1] == 0 &&  typeList[0] == 0 ){
		if(sumnoise < 3){               // ��������50���Ĳ������α������3�������жϷ���
			if(meanrr - rr >  floor(0.17 * meanrr))     
				apstats = 2;       // ����ǰ�ʴ���0.17ʱ���жϵ�ǰ�Ĳ�Ϊ����
		}
	}
	lastwhetherDomType = whetherDomType;
	lastrr = rr;
	lastmean = meanrr;

	return apstats;
	
}


int stabmean(int rr) {
	int sumrr = 0;
	int meanrr =  mymedian(rr);           //�����10��rr���ڵľ�ֵ��Ϊ��ʼֵ
	double rumda = (double)( meanrr - rr ) / meanrr ;
	if (abs(rumda) < 0.05){             // ����RR���ڵĲ��������½����rr�������
		id ++;
		for(int i = 1;i < 5;i ++){
			selectedrr[5 - i] = selectedrr[4 - i];
			sumrr += selectedrr[5 - i];
		}
		sumrr += rr;
		selectedrr[0] = rr;	  

		if(id == 5)
			meanrr = sumrr / 5;              //���ֵ����Ϊ��ʼ�� meanrr
		else if(id > 5)
			meanrr = rr;                    //�Ƿ���ֱ�����ڽ���RR�������
	}
	else{
		if(rumda < -0.3 || rumda > 0.5 )             // �в�ͬѰ����RR���ڲ�����������߹�С
			meanrr = lastmeanrr;                     //������һ��rr���ڣ�������ǰ��rr����
		else
			meanrr = ((7*lastmeanrr) >> 3) + ((lastrr + rr) >> 4) ; 			   // ������һ��������RR���ڣ�meanRR�������ĵ���	
	}
	lastmeanrr = meanrr;
	lastrr = rr;
	return meanrr;
}

int mymedian(int rr) {
	static int count =0;
	static int rrinterval[10] = {0};
	count ++;
	int sum = 0;
	int meanrr;
	for(int i = 1;i < 10;i ++){
		rrinterval[10 - i] = rrinterval[9 - i];
		sum += rrinterval[10 - i];
	}
	rrinterval[0] = rr;	
	sum += rr;

	if(count < 10)
		meanrr = sum / count;
	else
	meanrr = sum / 10;

	return meanrr;
}

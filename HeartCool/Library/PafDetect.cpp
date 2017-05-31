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
int PafDetect(float rr,int type, int whetherDomType){//rlocation 是监测出的R点的位置，单位为毫秒
    
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
		//  当前心搏和前一个心搏都为正常主导心博
	if( lastwhetherDomType == 1 && whetherDomType == 1 && typeList[1] == 0 &&  typeList[0] == 0 ){
		if(sumnoise < 3){               // 如果最近的50个心搏里面的伪差少于3个，才判断房早
			if(meanrr - rr >  floor(0.17 * meanrr))     
				apstats = 2;       // 当提前率大于0.17时，判断当前心搏为房早
		}
	}
	lastwhetherDomType = whetherDomType;
	lastrr = rr;
	lastmean = meanrr;

	return apstats;
	
}


int stabmean(int rr) {
	int sumrr = 0;
	int meanrr =  mymedian(rr);           //求最近10个rr间期的均值作为初始值
	double rumda = (double)( meanrr - rr ) / meanrr ;
	if (abs(rumda) < 0.05){             // 正常RR间期的波动，让新进入的rr进入队列
		id ++;
		for(int i = 1;i < 5;i ++){
			selectedrr[5 - i] = selectedrr[4 - i];
			sumrr += selectedrr[5 - i];
		}
		sumrr += rr;
		selectedrr[0] = rr;	  

		if(id == 5)
			meanrr = sumrr / 5;              //求均值，作为起始的 meanrr
		else if(id > 5)
			meanrr = rr;                    //是否房早直接与邻近的RR间期相关
	}
	else{
		if(rumda < -0.3 || rumda > 0.5 )             // 有不同寻常的RR间期波动，过大或者过小
			meanrr = lastmeanrr;                     //利用上一个rr间期，放弃当前的rr间期
		else
			meanrr = ((7*lastmeanrr) >> 3) + ((lastrr + rr) >> 4) ; 			   // 对于有一定搏动的RR间期，meanRR做谨慎的调整	
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

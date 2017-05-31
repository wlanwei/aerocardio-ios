/*****************************************************************************
FILE:  wavefilter.cpp
AUTHOR:	wan 
CREATED:	2/16/2017
  ___________________________________________________________________________

  wavefilter.cpp filter functions to aid denoise in electrocardiograms.
  __________________________________________________________________________
*******************************************************************************/
#include "WAVEFilter.h"

/*
int medFilter(int ,int,int ); 
int firlpFilter( int,int,int);
int smoothFilter( int, int,int );
int smoothstFilter( int, int,int );

void ShellSort(int array[],int n);
void swap(int array[],int i,int j);
*/

/******************************************************************************
* Syntax:
*	WAVEFilter(int datum,int init)
* Description:
*
*	The filter buffers and static variables are reset if a value other than
*	0 is passed to QRSFilter through init.
*******************************************************************************/
//BaseFilter::BaseFilter(){}
//BaseFilter::BaseFilter(int datum,int init,int num){
//  theIn =  datum;
//  theInit = init;
//  theNum = num;
//}
//BaseFilter::~BaseFilter(){}

#define LeadNum  3
int WAVEFilter(int datum, int init,int num)
	{
	int fdatumInt = 0;
	int fdatumD = 0 ;

	if(init)
		{
		medFilter(0,1,0);            // Median Filter
        firlpFilter(0,1,0) ;          // FIR low pass filter
        smoothFilter(0, 1 ,0) ;      // 
        smoothstFilter(0, 1,0 ) ;    // 
		}

	fdatumInt = medFilter( datum, 0 ,num) ;		// Low pass filter data.
	fdatumD = firlpFilter( fdatumInt, 0,num ) ;	// High pass filter data.
	fdatumD = smoothFilter( fdatumD, 0,num ) ;	// Take the derivative.
	fdatumD = smoothstFilter( fdatumD, 0 ,num) ;	// Average over an 80 ms window .
	return(fdatumD) ;
	}
/*************************************************************************
*  medFilter() 中值滤波，去除脉冲噪声
**************************************************************************/
int medFilter( int datum ,int init,int num)
	{
	//static long y1 = 0, y2 = 0 ;
	static int data[LeadNum][MEDFILTER_ORDER+1] ={0};
	int dataT[MEDFILTER_ORDER+1] = {0};
	/*long y0 ;
	int output, halfPtr ;*/
	int output;
	if(init)
		{
			for(int i=0; i<LeadNum;i++)
		       for(int ptr = 0; ptr < MEDFILTER_ORDER+1; ++ptr)
			       data[i][ptr] = 0 ;
		}

	data[num][MEDFILTER_ORDER] = datum;   //  input data
	memcpy(dataT,data[num],sizeof(int)*(MEDFILTER_ORDER+1));
	ShellSort(dataT,MEDFILTER_ORDER+1);
	output = dataT[MEDFILTER_ORDER/2];
	for(int ii= 1; ii<MEDFILTER_ORDER+1;ii++){
		data[num][ii-1] = data[num][ii];
	}
	return(output) ;
	}

/*************************************************************************
*  firlpFilter() 45Hz低通滤波器
**************************************************************************/

int firlpFilter( int datum ,int init,int num)
	{   
        float output = 0;
		static float data[LeadNum][FIRLP_ORDER] ={0};

		if(init)
		{
			for(int i=0; i<LeadNum;i++)
		       for(int ptr = 0; ptr < FIRLP_ORDER; ++ptr)
			       data[i][ptr] = 0 ;
		}

		
		for (int n = 0; n < FIRLP_ORDER; n ++)
		{
               output += FIRLP_PARA[n+1]*data[num][n];
		}
		       output += FIRLP_PARA[0]*datum;
		for (int m = FIRLP_ORDER-1; m >0  ; m --) {
			data[num][m] = data[num][m-1];
		}
		data[num][0] = datum;
		return((int)output) ;
	}
/******************************************************************************
*  smoothFilter 去基漂算法
******************************************************************************/
int smoothFilter( int datum, int init,int num )
	{
		float output = 0;
		static float sumDataOne[LeadNum] = {0};
		static float sumDataTwo[LeadNum] = {0};
		static float dataOne[LeadNum][SMOOTH_ORDER_ONE+1] = {0};
		static float dataTwo[LeadNum][SMOOTH_ORDER_TWO+1] = {0};

		if(init)
		{
			for(int i=0; i<LeadNum;i++){
		       for(int ptr = 0; ptr < SMOOTH_ORDER_ONE+1; ++ptr)
			       dataOne[i][ptr] = 0 ;
			   for(int ptr = 0; ptr < SMOOTH_ORDER_TWO+1; ++ptr)
			       dataTwo[i][ptr] = 0 ;

			}
		}

		dataOne[num][SMOOTH_ORDER_ONE] = datum;
		sumDataOne[num] += datum;
		output =dataOne[num][SMOOTH_ORDER_ONE/2] - sumDataOne[num]/(SMOOTH_ORDER_ONE+1);
		sumDataOne[num] -=dataOne[num][0];

		dataTwo[num][SMOOTH_ORDER_TWO] = output;
		sumDataTwo[num] += output;
		output =dataTwo[num][SMOOTH_ORDER_TWO/2] - sumDataTwo[num]/(SMOOTH_ORDER_TWO+1);
		sumDataTwo[num] -=dataTwo[num][0];

		for (int m = 1; m <SMOOTH_ORDER_ONE+1  ; m ++) {
			dataOne[num][m-1] = dataOne[num][m];
		}

		for (int m = 1; m <SMOOTH_ORDER_TWO+1  ; m ++) {
			dataTwo[num][m-1] = dataTwo[num][m];
		}
		
		return((int)output) ;
	}

/*****************************************************************************
*  smoothstFilter :七点三次平滑
*****************************************************************************/

int smoothstFilter( int datum, int init,int num) 
	{
	    float output = 0;
		static float data[LeadNum][SMOOTH_SEVEN_THREE_FILTER_ORDER] ={0};
		if(init)
		{
			for(int i=0; i<LeadNum;i++){
		       for(int ptr = 0; ptr < SMOOTH_SEVEN_THREE_FILTER_ORDER; ++ptr)
			       data[i][ptr] = 0 ;
			}
		}
		data[num][SMOOTH_SEVEN_THREE_FILTER_ORDER-1] =datum; 
		output=( -2.0 * (data[num][0] + data[num][6]) + 3.0 * (data[num][1] + data[num][5]) +
                        6.0 * (data[num][2] + data[num][4]) + 7.0 * data[num][3] ) / 21.0;

		for (int m = 1; m < SMOOTH_SEVEN_THREE_FILTER_ORDER; m ++) {
			data[num][m-1] = data[num][m];
		}
		return((int)output) ;
	}
/*交换函数，作用是交换数组中的两个元素的位置*/  
void swap(int array[],int i,int j)  
{  
    int tmp=array[i];  
    array[i]=array[j];  
    array[j]=tmp;  
}  
  
/*希尔排序*/  
void ShellSort(int array[],int n)  
{  
    for(int delta=n/2;delta>0;delta/=2)  
    {  
        for(int i=0;i<delta;i++)  
        {  
            for(int j=i+delta;j<n;j+=delta)  
            {  
                for(int k=j;k>0;k-=delta)  
                {  
                    if(array[k]<array[k-1])  
                        swap(array,k,k-1);  
                }  
            }  
        }  
    }  
}  

#ifndef _WAVEFILTER_
#define _WAVEFILETER_

//#include "BaseFilter.h"
#include <math.h>
#include <iostream>
#include "WAVEPARA.h"
#include <string.h>


int WAVEFilter(int datum, int init,int num);

int medFilter( int datum ,int init,int num);
int firlpFilter( int datum ,int init,int num);

int smoothFilter( int datum, int init,int num );

int smoothstFilter( int datum, int init,int num);
void ShellSort(int array[],int n);
void swap(int array[],int i,int j);
#endif

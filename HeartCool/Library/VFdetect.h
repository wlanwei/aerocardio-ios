#ifndef _VFDETECT_
#define _VFDETECT_

#include <math.h>
#include <iostream>
#include "WAVEPARA.h"
#include <cmath>

double getMax(double *P,int len);
double getMin(double *P,int len);

void ShellSort(int array[],int n);
void swap(int array[],int i,int j);

bool VFdetect (double datum);
#endif

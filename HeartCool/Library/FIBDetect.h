#ifndef _FIBDETECT_
#define _FIBDETECT_

#include "EcgMark.h"
#include "WAVEPARA.h"
#include <math.h>
//#include <cmath>
#include <vector>


using namespace std;
float getMax(float *P,int len);
float getMin(float *P,int len);
int roundInt( float val);
float getErf(float mean, float std,float data);
float getIngeral(float x0,float xn);
EcgMark FIBDetect(float rr,long currentTime,int type);
#endif

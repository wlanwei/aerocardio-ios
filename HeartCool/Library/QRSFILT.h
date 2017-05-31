#ifndef _QRSFILT_
#define _QRSFILT_

#include <math.h>
#include "QRSDET.h"
#include <stdlib.h>

// Local Prototypes.

int lpfilt( int datum ,int init) ;
int hpfilt( int datum, int init ) ;
int deriv1( int x0, int init ) ;
int deriv2( int x0, int init ) ;
int mvwint(int datum, int init) ;
int QRSFilter(int datum,int init);

#endif
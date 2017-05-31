/*****************************************************************************
FILE:  rythmchk.h
AUTHOR:	Patrick S. Hamilton
REVISED by wan 0n 2017/2/16
******************************************************************************/
#ifndef _RYTHMCHY_
#define _RYTHMCHY_

#include "QRSDET.h"		// For time intervals.
#include "ECGCODES.h"	// Defines codes of NORMAL, PVC, and UNKNOWN.
#include <stdlib.h>		// For abs()

// Define RR interval types.

#define QQ	0	// Unknown-Unknown interval.
#define NN	1	// Normal-Normal interval.
#define NV	2	// Normal-PVC interval.
#define VN	3	// PVC-Normal interval.
#define VV	4	// PVC-PVC interval.

#define RBB_LENGTH	8
#define LEARNING	0
#define READY	1

#define BRADY_LIMIT	MS1500

// Local prototypes.
int RRMatch(int rr0,int rr1) ;
int RRShort(int rr0,int rr1) ;
int RRShort2(int *rrIntervals, int *rrTypes) ;
int RRMatch2(int rr0,int rr1) ;
void ResetRhythmChk(void) ;
int RhythmChk(int rr) ;
int IsBigeminy(void) ;

#endif


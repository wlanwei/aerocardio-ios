#ifndef _CLASSIFY_
#define _CLASSIFY_

#include <iostream>
#include <stdlib.h>	// For abs()
#include <stdio.h>
#include "ECGCODES.h"
#include "QRSDET.h"	// For base sample rate.
#include "BDAC.h"
#include "MATCH.h"
#include "RYTHMCHK.h"
#include "ANALBEAT.h"
#include "POSTCLAS.h"


#define MATCH_LIMIT 1.3					// Threshold for template matching
												// without amplitude sensitivity.
#define MATCH_WITH_AMP_LIMIT	2.5	// Threshold for matching index that
												// is amplitude sensitive.
#define PVC_MATCH_WITH_AMP_LIMIT 0.9 // Amplitude sensitive limit for
												 //matching premature beats
#define BL_SHIFT_LIMIT	100			// Threshold for assuming a baseline shift.
#define NEW_TYPE_NOISE_THRESHOLD	18	// Above this noise level, do not create
												// new beat types.
#define NEW_TYPE_HF_NOISE_LIMIT 75	// Above this noise level, do not crate
												// new beat types.

#define MATCH_NOISE_THRESHOLD	0.7	// Match threshold below which noise
												// indications are ignored.

// TempClass classification rule parameters.

#define R2_DI_THRESHOLD 1.0		// Rule 2 dominant similarity index threshold
#define R3_WIDTH_THRESHOLD	BEAT_MS90		// Rule 3 width threshold.
#define R7_DI_THRESHOLD	1.2		// Rule 7 dominant similarity index threshold
#define R8_DI_THRESHOLD 1.5		// Rule 8 dominant similarity index threshold
#define R9_DI_THRESHOLD	2.0		// Rule 9 dominant similarity index threshold
#define R10_BC_LIM	3				// Rule 10 beat count limit.
#define R10_DI_THRESHOLD	2.5	// Rule 10 dominant similarity index threshold
#define R11_MIN_WIDTH BEAT_MS110			// Rule 11 minimum width threshold.
#define R11_WIDTH_BREAK	BEAT_MS140			// Rule 11 width break.
#define R11_WIDTH_DIFF1	BEAT_MS40			// Rule 11 width difference threshold 1
#define R11_WIDTH_DIFF2	BEAT_MS60			// Rule 11 width difference threshold 2
#define R11_HF_THRESHOLD	45		// Rule 11 high frequency noise threshold.
#define R11_MA_THRESHOLD	14		// Rule 11 motion artifact threshold.
#define R11_BC_LIM	1				// Rule 11 beat count limit.
#define R15_DI_THRESHOLD	3.5	// Rule 15 dominant similarity index threshold
#define R15_WIDTH_THRESHOLD BEAT_MS100	// Rule 15 width threshold.
#define R16_WIDTH_THRESHOLD BEAT_MS100	// Rule 16 width threshold.
#define R17_WIDTH_DELTA	BEAT_MS20			// Rule 17 difference threshold.
#define R18_DI_THRESHOLD	1.5	// Rule 18 dominant similarity index threshold.
#define R19_HF_THRESHOLD	75		// Rule 19 high frequency noise threshold.

// Dominant monitor constants.

#define DM_BUFFER_LENGTH	180
#define IRREG_RR_LIMIT	60
#define AVELENGTH	BEAT_MS50
// Local prototypes.

int HFNoiseCheck(int *beat) ;
int TempClass(int rhythmClass, int morphType, int beatWidth, int domWidth,
	int domType, int hfNoise, int noiseLevel, int blShift, double domIndex) ;
int DomMonitor(int morphType, int rhythmClass, int beatWidth, int rr, int reset) ;
int GetDomRhythm(void) ;
int GetRunCount(void) ;
void DownSampleBeat(int *beatOut, int *beatIn);
// Local Global variables
using namespace std;
int* Classify(int *tempBeat,int rr, int noiseLevel,int init) ;

void AdjustDomData(int oldType, int newType);
void CombineDomData(int oldType, int newType);
int GetNewDominantType(void);
#endif
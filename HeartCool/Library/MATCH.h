/*****************************************************************************
FILE:  match.h
AUTHOR:	Patrick S. Hamilton
REVISED:	12/4/2001
match.h: Beat matching prototype definitions.
******************************************************************************/

#ifndef _MATCH_
#define _MATCH_

#include <stdlib.h>
#include <stdio.h>
#include "ECGCODES.h"

#include "BDAC.h"
#include "ANALBEAT.h"
#include "CLASSIFY.h"


#define MATCH_LENGTH	BEAT_MS300	// Number of points used for beat matching.
//#define MATCH_LIMIT	1.2			// Match limit used testing whether two
								// Limit used for deciding whether two types
											// can be combined.
			// beat types might be combined.
#define COMBINE_LIMIT	0.8		
#define MATCH_START	(FIDMARK-(MATCH_LENGTH/2))	// Starting point for beat matching
#define MATCH_END	(FIDMARK+(MATCH_LENGTH/2))		// End point for beat matching.
#define MAXPREV	8	// Number of preceeding beats used as beat features.
#define MAX_SHIFT	BEAT_MS40

// Local prototypes.

int NoiseCheck(int *beat) ;
double CompareBeats(int *beat1, int *beat2, int *shiftAdj) ;
double CompareBeats2(int *beat1, int *beat2, int *shiftAdj) ;
void UpdateBeat(int *aveBeat, int *newBeat, int shift) ;
void BeatCopy(int srcBeat, int destBeat) ;
int MinimumBeatVariation(int type) ;
/******************************************/

int NewBeatType(int *beat) ;
void BestMorphMatch(int *newBeat,int *matchType,double *matchIndex, double *mi2, int *shiftAdj) ;
void UpdateBeatType(int matchType,int *newBeat, double mi2, int shiftAdj) ;
int GetTypesCount(void) ;
int GetBeatTypeCount(int type) ;
int IsTypeIsolated(int type) ;
void SetBeatClass(int type, int beatClass) ;
int GetBeatClass(int type) ;
int GetDominantType(void) ;
int GetBeatWidth(int type) ;
int GetPolarity(int type) ;
int GetRhythmIndex(int type) ;
void ResetMatch(void) ;
void ClearLastNewType(void) ;
int GetBeatBegin(int type) ;
int GetBeatEnd(int type) ;
int GetBeatAmp(int type) ;
int MinimumBeatVariation(int type) ;
int GetBeatCenter(int type) ;
int WideBeatVariation(int type) ;
double DomCompare2(int *newBeat, int domType) ;
double DomCompare(int newType, int domType) ;
#endif

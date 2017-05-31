/*****************************************************************************
FILE:  postclas.cpp
AUTHOR:	Patrick S. Hamilton
revised by wan on 2017/2/24
****************************************************************/

#include "POSTCLAS.h"


// Records of post classifications.

int PostClass[MAXTYPES][8], PCInitCount = 0 ;
int PCRhythm[MAXTYPES][8] ;

/**********************************************************************
 Resets post classifications for beats.
**********************************************************************/

void ResetPostClassify()
	{
	int i, j ;
	for(i = 0; i < MAXTYPES; ++i)
		for(j = 0; j < 8; ++j)
			{
			PostClass[i][j] = 0 ;
			PCRhythm[i][j] = 0 ;
			}
	PCInitCount = 0 ;
	}

/***********************************************************************
	Classify the previous beat type and rhythm type based on this beat
	and the preceding beat.  This classifier is more sensitive
	to detecting premature beats followed by compensitory pauses.
************************************************************************/

void PostClassify(int *recentTypes, int domType, int *recentRRs, int width, double mi2,
	int rhythmClass)
	{
	static int lastRC, lastWidth ;
	static double lastMI2 ;
	int i, regCount, pvcCount, normRR ;
	double mi3 ;

	// If the preceeding and following beats are the same type,
	// they are generally regular, and reasonably close in shape
	// to the dominant type, consider them to be dominant.

	if((recentTypes[0] == recentTypes[2]) && (recentTypes[0] != domType)
		&& (recentTypes[0] != recentTypes[1]))
		{
		mi3 = DomCompare(recentTypes[0],domType) ;
		for(i = regCount = 0; i < 8; ++i)
			if(PCRhythm[recentTypes[0]][i] == NORMAL)
				++regCount ;
		if((mi3 < 2.0) && (regCount > 6))
			domType = recentTypes[0] ;
		}

	// Don't do anything until four beats have gone by.

	if(PCInitCount < 3)
		{
		++PCInitCount ;
		lastWidth = width ;
		lastMI2 = 0 ;
		lastRC = 0 ;
		return ;
		}

	if(recentTypes[1] < MAXTYPES)
		{

		// Find first NN interval.
		for(i = 2; (i < 7) && (recentTypes[i] != recentTypes[i+1]); ++i) ;
		if(i == 7) normRR = 0 ;
		else normRR = recentRRs[i] ;

		// Shift the previous beat classifications to make room for the
		// new classification.
		for(i = pvcCount = 0; i < 8; ++i)
			if(PostClass[recentTypes[1]][i] == PVC)
				++pvcCount ;

		for(i = 7; i > 0; --i)
			{
			PostClass[recentTypes[1]][i] = PostClass[recentTypes[1]][i-1] ;
			PCRhythm[recentTypes[1]][i] = PCRhythm[recentTypes[1]][i-1] ;
			}

		// If the beat is premature followed by a compensitory pause and the
		// previous and following beats are normal, post classify as
		// a PVC.

		if(((normRR-(normRR>>3)) >= recentRRs[1]) && ((recentRRs[0]-(recentRRs[0]>>3)) >= normRR)// && (lastMI2 > 3)
			&& (recentTypes[0] == domType) && (recentTypes[2] == domType)
				&& (recentTypes[1] != domType))
			PostClass[recentTypes[1]][0] = PVC ;

		// If previous two were classified as PVCs, and this is at least slightly
		// premature, classify as a PVC.

		else if(((normRR-(normRR>>4)) > recentRRs[1]) && ((normRR+(normRR>>4)) < recentRRs[0]) &&
			(((PostClass[recentTypes[1]][1] == PVC) && (PostClass[recentTypes[1]][2] == PVC)) ||
				(pvcCount >= 6) ) &&
			(recentTypes[0] == domType) && (recentTypes[2] == domType) && (recentTypes[1] != domType))
			PostClass[recentTypes[1]][0] = PVC ;

		// If the previous and following beats are the dominant beat type,
		// and this beat is significantly different from the dominant,
		// call it a PVC.

		else if((recentTypes[0] == domType) && (recentTypes[2] == domType) && (lastMI2 > 2.5))
			PostClass[recentTypes[1]][0] = PVC ;

		// Otherwise post classify this beat as UNKNOWN.

		else PostClass[recentTypes[1]][0] = UNKNOWN ;

		// If the beat is premature followed by a compensitory pause, post
		// classify the rhythm as PVC.

		if(((normRR-(normRR>>3)) > recentRRs[1]) && ((recentRRs[0]-(recentRRs[0]>>3)) > normRR))
			PCRhythm[recentTypes[1]][0] = PVC ;

		// Otherwise, post classify the rhythm as the same as the
		// regular rhythm classification.

		else PCRhythm[recentTypes[1]][0] = lastRC ;
		}

	lastWidth = width ;
	lastMI2 = mi2 ;
	lastRC = rhythmClass ;
	}


/*************************************************************************
	CheckPostClass checks to see if three of the last four or six of the
	last eight of a given beat type have been post classified as PVC.
*************************************************************************/

int CheckPostClass(int type)
	{
	int i, pvcs4 = 0, pvcs8 ;

	if(type == MAXTYPES)
		return(UNKNOWN) ;

	for(i = 0; i < 4; ++i)
		if(PostClass[type][i] == PVC)
			++pvcs4 ;
	for(pvcs8=pvcs4; i < 8; ++i)
		if(PostClass[type][i] == PVC)
			++pvcs8 ;

	if((pvcs4 >= 3) || (pvcs8 >= 6))
		return(PVC) ;
	else return(UNKNOWN) ;
	}

/****************************************************************************
	Check classification of previous beats' rhythms based on post beat
	classification.  If 7 of 8 previous beats were classified as NORMAL
	(regular) classify the beat type as NORMAL (regular).
	Call it a PVC if 2 of the last 8 were regular.
****************************************************************************/

int CheckPCRhythm(int type)
	{
	int i, normCount, n ;


	if(type == MAXTYPES)
		return(UNKNOWN) ;

	if(GetBeatTypeCount(type) < 9)
		n = GetBeatTypeCount(type)-1 ;
	else n = 8 ;

	for(i = normCount = 0; i < n; ++i)
		if(PCRhythm[type][i] == NORMAL)
			++normCount;
	if(normCount >= 7)
		return(NORMAL) ;
	if(((normCount == 0) && (n < 4)) ||
		((normCount <= 1) && (n >= 4) && (n < 7)) ||
		((normCount <= 2) && (n >= 7)))
		return(PVC) ;
	return(UNKNOWN) ;
	}

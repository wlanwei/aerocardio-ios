/*****************************************************************************
FILE:  analbeat.cpp
AUTHOR:	Patrick S. Hamilton
revised by wan on 2017/2/24

*****************************************************************************/
#include "ANALBEAT.h"

/****************************************************************
	IsoCheck determines whether the amplitudes of a run
	of data fall within a sufficiently small amplitude that
	the run can be considered isoelectric.
*****************************************************************/

int IsoCheck(int *data, int isoLength)
	{
	int i, max, min ;

	for(i = 1, max=min = data[0]; i < isoLength; ++i)
		{
		if(data[i] > max)
			max = data[i] ;
		else if(data[i] < min)
			min = data[i] ;
		}

	if(max - min < ISO_LIMIT)
		return(1) ;
   return(0) ;
	}

/**********************************************************************
	AnalyzeBeat takes a beat buffer as input and returns (via pointers)
	estimates of the QRS onset, QRS offset, polarity, isoelectric level
	beat beginning (P-wave onset), and beat ending (T-wave offset).
	Analyze Beat assumes that the beat has been sampled at 100 Hz, is
	BEATLGTH long, and has an R-wave location of roughly FIDMARK.

	Note that beatBegin is the number of samples before FIDMARK that
	the beat begins and beatEnd is the number of samples after the
	FIDMARK that the beat ends.
************************************************************************/

int* AnalyzeBeat(int *beat)
	{
	int * detect = new int[14]; 
	int isobeat8[8];
	int isobeat5[5];
	int maxSlope = 0, maxSlopeI, minSlope = 0, minSlopeI  ;
	int onset = 0, offset = 0 , amp = 0, beatBegin = 0, beatEnd = 0;
	int maxV, minV,flag=0;
	int isoStart, isoEnd ;
	int slope, i ,k;
	int Xmax1, Xmin1,maxV1, minV1, Xmax2, Xmin2,maxV2, minV2;
	int isoLevel ,Rset ;
	// Search back from the fiducial mark to find the isoelectric
	// region preceeding the QRS complex.

	i = 32;
		for(k = 0;k < 8;k ++)
			isobeat8[k] = beat[i + k]; 

		while (i > 0 && IsoCheck(isobeat8,8) == 0){	
			for(k = 0;k < 8;k ++)
				isobeat8[k] = beat[i + k]; 	
			--i ;
		}

		if(i == 0){
			i = 35;
			for(k = 0;k < 5;k ++)
				isobeat5[k] = beat[i + k]; 

			while (i > 0 && IsoCheck(isobeat5,5) == 0){				
				for(k = 0;k < 5;k ++)
					isobeat5[k] = beat[i + k]; 	
				--i ;
			}
			isoStart = i + 4 ;		    			
		}
		else
			isoStart = i + 7 ;
	//for(i = FIDMARK-ISO_LENGTH2; (i > 0) && (IsoCheck(&beat[i],ISO_LENGTH2) == 0); --i) ;
	// If the first search didn't turn up any isoelectric region, look for
	// a shorter isoelectric region.
	//if(i == 0)
	//	{
	//	for(i = FIDMARK-ISO_LENGTH1; (i > 0) && (IsoCheck(&beat[i],ISO_LENGTH1) == 0); --i) ;
	//	isoStart = i + (ISO_LENGTH1 - 1) ;
	//	}
	//else isoStart = i + (ISO_LENGTH2 - 1) ;

	// Search forward from the R-wave to find an isoelectric region following
	// the QRS complex.

	/*for(i = FIDMARK; (i < BEATLGTH) && (IsoCheck(&beat[i],ISO_LENGTH1) == 0); ++i) ;
	isoEnd = i ;*/
        i = FIDMARK;
		for(k = 0;k < 5 ;k ++)
			isobeat5[k] = beat[i + k]; 
		while(i < (BEATLGTH - 5) && IsoCheck(isobeat5,5) == 0){       // on 20160426			
			for(k = 0;k < 5 ;k ++)
				isobeat5[k] = beat[i + k]; 
			++ i;
		}
		isoEnd = i ;
	// Find the maximum and minimum slopes on the
	// QRS complex.
/*************************************************************************************/
	i = FIDMARK-BEAT_MS150 ;
	maxSlope = maxSlope = beat[i] - beat[i-1] ;
	maxSlopeI = minSlopeI = i ;

	for(; i < FIDMARK+BEAT_MS150; ++i)
		{
		slope = beat[i] - beat[i-1] ;
		if(slope > maxSlope)
			{
			maxSlope = slope ;
			maxSlopeI = i ;
			}
		else if(slope < minSlope)
			{
			minSlope = slope ;
			minSlopeI = i ;
			}
		}

	// Use the smallest of max or min slope for search parameters.

	if(maxSlope > -minSlope)
		maxSlope = -minSlope ;
	else minSlope = -maxSlope ;

	if(maxSlopeI < minSlopeI)
		{

		// Search back from the maximum slope point for the QRS onset.

		for(i = maxSlopeI;(i > 0) && ((beat[i]-beat[i-1]) > (maxSlope >> 2)); --i) ;
			onset = i-1 ;

		// Check to see if this was just a brief inflection.

		for(; (i > onset-INF_CHK_N) && ((beat[i]-beat[i-1]) <= (maxSlope >>2)); --i) ;
		if(i > onset-INF_CHK_N)
			{
			for(;(i > 0) && ((beat[i]-beat[i-1]) > (maxSlope >> 2)); --i) ;
			onset = i-1 ;
			}
		i = onset+1 ;

		// Check to see if a large negative slope follows an inflection.
		// If so, extend the onset a little more.

		for(;(i > onset-INF_CHK_N) && ((beat[i-1]-beat[i]) < (maxSlope>>2)); --i);
		if(i > onset-INF_CHK_N)
			{
			for(; (i > 0) && ((beat[i-1]-beat[i]) > (maxSlope>>2)); --i) ;
			onset = i-1 ;
			}

		// Search forward from minimum slope point for QRS offset.

		for(i = minSlopeI;(i > 0)&&(i < BEATLGTH) && ((beat[i] - beat[i-1]) < (minSlope >>2)); ++i);
			offset = i ;

		// Make sure this wasn't just an inflection.

		for(; (i > 0)&&(i < offset+INF_CHK_N) && ((beat[i]-beat[i-1]) >= (minSlope>>2)); ++i) ;
		if(i < offset+INF_CHK_N)
			{
			for(;(i > 0)&&(i < BEATLGTH) && ((beat[i]-beat[i-1]) < (minSlope >>2)); ++i) ;
			offset = i ;
			}
		i = offset ;

		// Check to see if there is a significant upslope following
		// the end of the down slope.

		for(;(i > 0) &&(i < offset+BEAT_MS40) && ((beat[i-1]-beat[i]) > (minSlope>>2)); ++i);
		if(i < offset+BEAT_MS40)
			{
			for(; (i > 0) &&(i < BEATLGTH) && ((beat[i-1]-beat[i]) < (minSlope>>2)); ++i) ;
			offset = i ;

			// One more search motivated by PVC shape in 123.

			for(; (i > 0) &&(i < offset+BEAT_MS60) && (beat[i]-beat[i-1] > (minSlope>>2)); ++i) ;
			if(i < offset + BEAT_MS60)
				{
				for(;(i > 0) &&(i < BEATLGTH) && (beat[i]-beat[i-1] < (minSlope>>2)); ++i) ;
				offset = i ;
				}
			}
		}

	else
		{

		// Search back from the minimum slope point for the QRS onset.

		for(i = minSlopeI;(i > 0) && ((beat[i]-beat[i-1]) < (minSlope >> 2)); --i) ;
			onset = i-1 ;

		// Check to see if this was just a brief inflection.

		for(; (i > 0) &&(i > onset-INF_CHK_N) && ((beat[i]-beat[i-1]) >= (minSlope>>2)); --i) ;
		if(i > onset-INF_CHK_N)
			{
			for(; (i > 0) && ((beat[i]-beat[i-1]) < (minSlope>>2));--i) ;
			onset = i-1 ;
			}
		i = onset+1 ;

		// Check for significant positive slope after a turning point.

		for(;(i > 0) &&(i > onset-INF_CHK_N) && ((beat[i-1]-beat[i]) > (minSlope>>2)); --i);
		if(i > onset-INF_CHK_N)
			{
			for(; (i > 0) && ((beat[i-1]-beat[i]) < (minSlope>>2)); --i) ;
			onset = i-1 ;
			}

		// Search forward from maximum slope point for QRS offset.

		for(i = maxSlopeI;(i > 0) &&(i < BEATLGTH) && ((beat[i] - beat[i-1]) > (maxSlope >>2)); ++i) ;
		offset = i ;

		// Check to see if this was just a brief inflection.

		for(; (i > 0) &&(i < offset+INF_CHK_N) && ((beat[i] - beat[i-1]) <= (maxSlope >> 2)); ++i) ;
		if(i < offset+INF_CHK_N)
			{
			for(;(i > 0) &&(i < BEATLGTH) && ((beat[i] - beat[i-1]) > (maxSlope >>2)); ++i) ;
			offset = i ;
			}
		i = offset ;

		// Check to see if there is a significant downslope following
		// the end of the up slope.

		for(;(i > 0) &&(i < offset+BEAT_MS40) && ((beat[i-1]-beat[i]) < (maxSlope>>2)); ++i);
		if(i < offset+BEAT_MS40)
			{
			for(; (i > 0) &&(i < BEATLGTH) && ((beat[i-1]-beat[i]) > (maxSlope>>2)); ++i) ;
			offset = i ;
			}
		}

	// If the estimate of the beginning of the isoelectric level was
	// at the beginning of the beat, use the slope based QRS onset point
	// as the iso electric point.

	if((isoStart == ISO_LENGTH1-1)&& (onset > isoStart)) // ** 4/19 **
		isoStart = onset ;

	// Otherwise, if the isoelectric start and the slope based points
	// are close, use the isoelectric start point.

	else if(onset-isoStart < BEAT_MS50)
		onset = isoStart ;

	// If the isoelectric end and the slope based QRS offset are close
	// use the isoelectic based point.

	if(isoEnd - offset < BEAT_MS50)
		offset = isoEnd ;

	isoLevel = beat[isoStart] ;


	// Find the maximum and minimum values in the QRS.

	for(i = onset, maxV1 = minV1 = beat[onset],Xmin1 = Xmax1 = i; i < (offset+onset)>>1; ++i){
		if(beat[i] > maxV1){
			maxV1 = beat[i] ;
			Xmax1 = i;
		}
		else if(beat[i] < minV1){
			minV1 = beat[i] ;
			Xmin1 = i;
		
		}
	}

	for(i = offset, maxV2 = minV2 = beat[offset],Xmin2 = Xmax2 = i; i >= (onset + offset)>>1 ; --i)
		{
			if(beat[i] > maxV2)
			{
				maxV2 = beat[i] ;
				Xmax2 = i;
			}
			else if(beat[i] < minV2)
			{
				minV2 = beat[i] ;
				Xmin2 = i;
			}
		}

		Rset =  Xmax1;
		flag = 1;                 //  R鍦ㄦ瀬澶�

		if(  Xmax1  > onset && Xmin2  < offset )
		{
			if (maxV1 < maxV2 && (offset - Xmax2 > (offset - onset) >> 3))
				Rset = Xmax2;
		}
		else if ((Xmin1 > onset  && Xmax2  < offset ) || (Xmax1 > onset  && Xmax2  < offset))
		{
			if( maxV1 < maxV2 )
				Rset = Xmax2;
		}
		else if( Xmin1 > onset  && Xmin2  < offset)
		{
			if( minV1 < minV2 )
				Rset = Xmin1;
			else
			{
				Rset = Xmin2;
			}
			flag = 0;        //  R鍦ㄦ瀬灏�

		}

		maxV = (maxV1>maxV2 ? maxV1:maxV2) ;
		minV = (minV1<minV2 ? minV1: minV2) ;
		amp = maxV-minV;

	// If the offset is significantly below the onset and the offset is
	// on a negative slope, add the next up slope to the width.

	if((beat[onset]-beat[offset] > ((maxV-minV)>>2)+((maxV-minV)>>3)))
		{

		// Find the maximum slope between the finish and the end of the buffer.

		for(i = maxSlopeI = offset, maxSlope = beat[offset] - beat[offset-1];
			(i < offset+BEAT_MS100) && (i < BEATLGTH); ++i)
			{
			slope = beat[i]-beat[i-1] ;
			if(slope > maxSlope)
				{
				maxSlope = slope ;
				maxSlopeI = i ;
				}
			}

		// Find the new offset.

		if(maxSlope > 0)
			{
			for(i = maxSlopeI;
				(i < BEATLGTH) && (beat[i]-beat[i-1] > (maxSlope>>1)); ++i) ;
			offset = i ;
			}
		}
		if(offset < 40){
				offset = 40 + (40 - offset);
			}

			i = 15;
			for(k = 0;k < 8;k ++)
				isobeat8[k] = beat[i + k - 8]; 

			while (i >= 8 && IsoCheck(isobeat8,8) == 0){			
				--i ;
				if(i >= 8){
					for(k = 0;k < 8;k ++)
						isobeat8[k] = beat[i + k - 8]; 
				}
			}

		//		for(i = FIDMARK- 25;(i >= 8) && (IsoCheck(beat[i-8],8) == 0); --i) ;
		beatBegin = i ;
	// Determine beginning and ending of the beat.
	// Search for an isoelectric region that precedes the R-wave.
	// by at least 250 ms.

	//for(i = FIDMARK-BEAT_MS250;
	//	(i >= BEAT_MS80) && (IsoCheck(&beat[i-BEAT_MS80],BEAT_MS80) == 0); --i) ;
	//*beatBegin = i ;

	// If there was an isoelectric section at 250 ms before the
	// R-wave, search forward for the isoelectric region closest
	// to the R-wave.  But leave at least 50 ms between beat begin
	// and onset, or else normal beat onset is within PVC QRS complexes.
	// that screws up noise estimation.

	if(beatBegin == FIDMARK-BEAT_MS250)
		{
		for(k = 0;k < 8;k ++)
				isobeat8[k] = beat[i + k - 8]; 

			while (i < onset- 5 && IsoCheck(isobeat8,8) == 1){
				++ i ;
				for(k = 0;k < 8;k ++)
					isobeat8[k] = beat[i + k - 8]; 

			}
			beatBegin = i-1 ;
		}

	// Rev 1.1
	else if(beatBegin == BEAT_MS80 - 1)
		{
		    int isoOn = (0> i - 8 ? 0:i-8);
			for(k = 0;k < 8;k ++){				
				isobeat8[k] = beat[isoOn + k];				
			}			

			while (i < onset && IsoCheck(isobeat8,8) == 0){	
				++ i ;
				isoOn = (0> i - 8 ? 0:i-8);                         // 鏇存柊 isoOn
				for(k = 0;k < 8;k ++)
					isobeat8[k] = beat[isoOn + k];		            // 鏇存柊 isobeat8
			}
			if(i < onset)
			{
				isoOn = (0> i - 8 ? 0:i-8);                            // 鏇存柊 isoOn
				for(k = 0;k < 8;k ++){					
					isobeat8[k] = beat[isoOn + k];	                      // 鏇存柊 isobeat8			
				}
				while (i < onset && IsoCheck(isobeat8,8) == 1){	
					++ i ;
					isoOn = (0>  i + k - 8 ? 0: i + k - 8);
					for(k = 0;k < 8;k ++){						
						isobeat8[k] = beat[isoOn + k];				
					}				
				}
				if(i < onset)
					beatBegin = i-1 ;
			}
		}

	// Search for the end of the beat as the first isoelectric
	// segment that follows the beat by at least 300 ms.

	i = FIDMARK+BEAT_MS300;
     for(k = 0;k < 8;k ++)
			isobeat8[k] = beat[i + k]; 
		while (i < 93 && IsoCheck(isobeat8,8) == 0){			
			for(k = 0;k < 8;k ++)
				isobeat8[k] = beat[i + k]; 	
			++ i ;
		}

	beatEnd = i ;

	detect[1] = onset;
	detect[2] = offset;
	detect[3] = Rset;
	detect[4] = amp;
	detect[5] = flag;      //  1: R鍦ㄦ瀬澶�    0: R鍦ㄦ瀬灏�
	detect[6] = isoLevel;  
	detect[7] = isoStart;
	detect[8] = isoEnd;
	detect[9] = maxSlope;
	return detect;
	// If the signal was isoelectric at 300 ms. search backwards.
/*	if(*beatEnd == FIDMARK+30)
		{
		for(; (i > *offset) && (IsoCheck(&beat[i],8) != 0); --i) ;
		*beatEnd = i ;
		}
*/
	// Calculate beat amplitude.

	/*maxV=minV=beat[*onset] ;
	for(i = *onset; i < *offset; ++i)
		if(beat[i] > maxV)
			maxV = beat[i] ;
		else if(beat[i] < minV)
			minV = beat[i] ;
	*amp = maxV-minV ;*/

	}





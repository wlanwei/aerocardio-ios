/*****************************************************************************
FILE:  bdac.cpp
AUTHOR:	Patrick S. Hamilton
revised by wan on 2017/2/24
bdac.cpp: Beat Detection And Classification
*******************************************************************************/
#include "BDAC.h"
//int gcd(int x, int y) ;

// Global Variables

static int ECGBuffer[ECG_BUFFER_LENGTH];  // Circular data buffer.
static int BeatBuffer[BEATLGTH] ;
static int BeatQue[BEAT_QUE_LENGTH];  // Buffer of detection delays.
static int RRCount = 0,InitBeatFlag = 1 ,BeatQueCount = 0 ,SampleCount = 0, ECGBufferIndex = 0 ;

/******************************************************************************
	ResetBDAC() resets static variables required for beat detection and
	classification.
*******************************************************************************/

void ResetBDAC(void)
	{
	QRSDet(0,1) ;	// Reset the qrs detector
	RRCount = 0 ;
	Classify(BeatBuffer,0,0,1) ;
	InitBeatFlag = 1 ;
   BeatQueCount = 0 ;	// Flush the beat que.
	}

/*****************************************************************************
Syntax:
	int BeatDetectAndClassify(int ecgSample, int *beatType, *beatMatch) ;

Description:
	BeatDetectAndClassify() implements a beat detector and classifier.
	ECG samples are passed into BeatDetectAndClassify() one sample at a
	time.  BeatDetectAndClassify has been designed for a sample rate of
	200 Hz.  When a beat has been detected and classified the detection
	delay is returned and the beat classification is returned through the
	pointer *beatType.  For use in debugging, the number of the template
   that the beat was matched to is returned in via *beatMatch.

Returns
	BeatDetectAndClassify() returns 0 if no new beat has been detected and
	classified.  If a beat has been classified, BeatDetectAndClassify returns
	the number of samples since the approximate location of the R-wave.

****************************************************************************/

BEAT BeatDetectAndClassify(int ecgSample, int init)
{
    BEAT beat(0,0,0,0,0,0,0,0);
	if (init)   
		{
			ResetBDAC();
		}

	static int sample_500_data[5] ={0};
	static int circlecount = 0;
	int out_data =0;
	circlecount ++;
	int result = 0;
	if(circlecount  >5){
	   circlecount = 1;
	}
	
	sample_500_data[circlecount -1] = ecgSample;
	if(circlecount == 3){
	   out_data =(sample_500_data[1] >> 1) + ((sample_500_data[0] + sample_500_data[2]) >> 2);
	   SampleCount ++;
	   beat = beatAnalyzer(out_data);
	   return beat;
	}else if(circlecount == 5){
	   out_data = (sample_500_data[3] + sample_500_data[4])>>1;
	   beat = beatAnalyzer(out_data);
	   SampleCount ++;
	   return beat;
	}else{
	   return beat;
	}
}

/**
    beatAnalyzer has been designed for a sample rate of
	200 Hz.  When a beat has been detected and classified the detection
	delay is returned and the beat classification is returned through the
	pointer *beatType.  For use in debugging, the number of the template
   that the beat was matched to is returned in via *beatMatch.
*/
BEAT beatAnalyzer(int ecgSample)
{
	int detectDelay, rr = 0, i=0, j=0 ;
	int  beatBegin = 0, beatEnd = 0 , Rset = 0 , R = 40;
	int noiseEst = 0,  qrsBegin = 0, qrsEnd = 0;
	int domType =0 ;
	int fidAdj = 0 ;
	int tempBeat[SAMPLE_RATE];//[(SAMPLE_RATE/BEAT_SAMPLE_RATE)*BEATLGTH] ;
	BEAT beat(0,0,0,0,0,0,0,0);
	int* detect = new int[14];
	// Store new sample in the circular buffer.

	ECGBuffer[ECGBufferIndex] = ecgSample ;
	if(++ECGBufferIndex == ECG_BUFFER_LENGTH)
		ECGBufferIndex = 0 ;

	// Increment RRInterval count.

	++RRCount ;

	// Increment detection delays for any beats in the que.

	for(i = 0; i < BeatQueCount; ++i)
		++BeatQue[i] ;

	// Run the sample through the QRS detector.

	detectDelay = QRSDet(ecgSample,0) ;
	if(detectDelay != 0)
		{
		BeatQue[BeatQueCount] = detectDelay ;
		++BeatQueCount ;
		}

	// Return if no beat is ready for classification.

	if((BeatQue[0] < (BEATLGTH-FIDMARK)*(SAMPLE_RATE/BEAT_SAMPLE_RATE))
		|| (BeatQueCount == 0))
		{
		NoiseCheck(ecgSample,0,rr, beatBegin, beatEnd) ;	// Update noise check buffer
		beat.setDelay(0);
		return beat ;
		}

	// Otherwise classify the beat at the head of the que.

	rr = RRCount - BeatQue[0] ;	// Calculate the R-to-R interval
	detectDelay = RRCount = BeatQue[0] ;

	// Estimate low frequency noise in the beat.
	// Might want to move this into classify().

	domType = GetDominantType() ;
	if(domType == -1)
		{
		beatBegin = 50;//MS250 ;
		beatEnd = 60;//MS300 ;
		}
	else
		{
		beatBegin = (SAMPLE_RATE/BEAT_SAMPLE_RATE)*(FIDMARK-GetBeatBegin(domType)) ;
		beatEnd = (SAMPLE_RATE/BEAT_SAMPLE_RATE)*(GetBeatEnd(domType)-FIDMARK) ;
		}
	noiseEst = NoiseCheck(ecgSample,detectDelay,rr,beatBegin,beatEnd) ;
	// Copy the beat from the circular buffer to the beat buffer
	// and reduce the sample rate by averageing pairs of data
	// points.
	j = ECGBufferIndex - detectDelay - (SAMPLE_RATE/BEAT_SAMPLE_RATE)*FIDMARK ;
	while(j < 0) {j += ECG_BUFFER_LENGTH ;}

	for(i = 0; i < (SAMPLE_RATE/BEAT_SAMPLE_RATE)*BEATLGTH; ++i)
		{
		tempBeat[i] = ECGBuffer[j] ;
		if(++j == ECG_BUFFER_LENGTH)
			j = 0 ;
		}

	//DownSampleBeat(BeatBuffer,tempBeat) ;

	// Update the QUE.

	for(i = 0; i < BeatQueCount-1; ++i)
		BeatQue[i] = BeatQue[i+1] ;
	--BeatQueCount ;
   // Skip the first beat.
	
		detect = Classify(tempBeat,rr,noiseEst,0) ;
		fidAdj = SAMPLE_RATE/BEAT_SAMPLE_RATE *detect[10] ;

	// Ignore detection if the classifier decides that this
	// was the trailing edge of a PVC.

	/*if(*beatType == 100)
		{
		RRCount += rr ;
		return beat ;
		}
*/
	// Limit the fiducial mark adjustment in case of problems with
	// beat onset and offset estimation.

	if(fidAdj > MS80)
		fidAdj = MS80 ;
	else if(fidAdj < -MS80)
		fidAdj = -MS80 ;
	beat.setDelay(5*(detectDelay - fidAdj));      //  杞� ms 涓哄崟浣�
	beat.setOnset(5*(2 * detect[1] -79 - (fidAdj >> 1)));
	beat.seyOffset( 5*(2 * detect[2] -79 - (fidAdj >> 1)));	
	beat.setAdjustR(5*(2 * detect[3] -79 - fidAdj));
	beat.setBeatType(detect[11]);
	beat.setIsDomType(detect[12]);
	return beat ;
}

void DownSampleBeat(int *beatOut, int *beatIn)
	{
	int i ;

	for(i = 0; i < BEATLGTH; ++i)
		beatOut[i] = (beatIn[i<<1]+beatIn[(i<<1)+1])>>1 ;
	}















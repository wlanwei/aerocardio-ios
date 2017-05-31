/*****************************************************************************
FILE:  BEAT.h
AUTHOR:	uteamtec
Created:	2017/2/13
******************************************************************************/
#pragma once
#ifndef _BEAT_	/* avoid multiple definitions */
#define _BEAT_	/* avoid multiple definitions */

class BEAT{
  public: 
	    BEAT();
		BEAT(int delay,int onset,int offset,int adjustR,int amp,int beatType,int fidadj,int isDomType);
		~BEAT();

		void setDelay(int delay){
		    thedelay = delay;
		}
		void setOnset(int onset){
			theonset = onset;
		}
		void seyOffset(int offset){
		    theoffset = offset;
		}
		void setAdjustR(int adjustR){
		    theadjustR = adjustR;
		}
		void setAmp(int amp){
		    theamp = amp;
		}
		void setBeatType(int beatType){
		    thebeatType = beatType;
		}
		void setFidadj(int fidadj){
		   thefidadj = fidadj;
		}
		void setIsDomType(int isDomType){
		   theisDomType = isDomType;
		}
		int getDelay(){
		    return thedelay;
		}
		int getOnset(){
			return theonset;
		}
		int geyOffset(){
		    return theoffset;
		}
		int getAdjustR(){
		    return theadjustR;
		}
		int getAmp(){
		    return theamp;
		}
		int getBeatType(){
		    return thebeatType;
		}
		int getFidadj(){
		   return thefidadj;
		}
		int getIsDomType(){
		   return theisDomType;
		}

	private: 
	int thedelay;
	int theonset;
	int theoffset;
	int theadjustR;
	int theamp;
	int thebeatType;
	int thefidadj;
	int theisDomType;	
};

#endif

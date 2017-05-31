/*****************************************************************************
FILE:  EcgMark.h
AUTHOR:	uteamtec
Created:	2017/2/13
******************************************************************************/
#pragma once
#ifndef _ECGMARK_	/* avoid multiple definitions */
#define _ECGMARK_	/* avoid multiple definitions */

//#define int TYPE_GROUP_STATUS  1;
//#define int TYPE_GROUP_PHYSIO  2;
//
//#define int STATUS_LOWPOWER  1;
//#define int STATUS_LEADOFF  2;
//#define int STATUS_SHORT  3;
//#define int STATUS_CHARGING  4;
//#define int STATUS_UNPLUG  5;
//#define int STATUS_PLUG  6;
//
//#define int PHYSIO_HR  1;
//#define int PHYSIO_BR  2;
//#define int PHYSIO_NOISE  3;
//
//    //new mark for user input
//#define int PHYSIO_USERINPUT  4;
//#define int PHYSIO_ABNORMAL  5; // PVC
class EcgMark{
  public: 
		EcgMark(long initStartTime, long initStopTime, int initTypeGroup, int initType, int initValue);
		~EcgMark();
       // void setDeviceId(byte[] deviceId) {deviceId = deviceId;}
        void setDeviceType(int deviceType) {deviceType = deviceType;}
       // byte[] getDeviceId() {return deviceId; }
        int getType(){ return type;}
        int getTypeGroup() {return typeGroup;}
        long getStartTime() {return startTime;}
        long getStopTime() {return stopTime;}
        int getValue() {return value;}

	private: 
	//byte deviceId[];
	int deviceType;
    long startTime;
    long stopTime;
    int typeGroup;
    int type;
    int value;	
};

#endif

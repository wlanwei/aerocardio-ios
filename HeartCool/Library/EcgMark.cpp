#include "EcgMark.h"
EcgMark::EcgMark(long initStartTime, long initStopTime, int initTypeGroup, int initType, int initValue) {
            startTime = initStartTime;
            stopTime = initStopTime;
            typeGroup = initTypeGroup;
            type = initType;
            value = initValue;
		}
EcgMark::~EcgMark(){};
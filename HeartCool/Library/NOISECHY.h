#ifndef _NOISECHY_
#define _NOISECHY_

#include <stdlib.h>
#include "QRSDET.h"

#define NB_LENGTH	MS1500
#define NS_LENGTH	MS50


int GetNoiseEstimate();
int NoiseCheck(int datum, int delay, int RR, int beatBegin, int beatEnd);
#endif

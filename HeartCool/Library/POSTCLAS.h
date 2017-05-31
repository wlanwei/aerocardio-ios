#ifndef _POSTCLAS_
#define _POSTCLAS_
#include "BDAC.h"
#include "ECGCODES.h"
#include "MATCH.h"
void ResetPostClassify() ;
void PostClassify(int *recentTypes, int domType, int *recentRRs, int width, double mi2,
	int rhythmClass) ;
int CheckPostClass(int type) ;
int CheckPCRhythm(int type) ;
#endif

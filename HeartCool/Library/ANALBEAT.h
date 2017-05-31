/*****************************************************************************
FILE:  analbeat.h
AUTHOR:	Patrick S. Hamilton
REVISED by wan on 2017/2/24
*****************************************************************************/
#ifndef _ANALBEAT_
#define _ANALBEAT_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "BDAC.h"

#define ISO_LENGTH1  BEAT_MS50
#define ISO_LENGTH2	BEAT_MS80
#define ISO_LIMIT	20
#define INF_CHK_N	BEAT_MS40

int IsoCheck(int *data, int isoLength);
int* AnalyzeBeat(int *beat);
#endif

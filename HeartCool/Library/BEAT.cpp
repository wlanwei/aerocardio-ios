#include "BEAT.h"
BEAT::BEAT(){}
BEAT::BEAT(int delay,int onset,int offset,int adjustR,int amp,int beatType,int fidadj,int isDomType){
		thedelay = delay;
		theonset = onset;
		theoffset = offset;
		theadjustR = adjustR;
		theamp = amp;
		thebeatType = beatType;
		thefidadj = fidadj;
		theisDomType = isDomType;
		}
BEAT::~BEAT(){}
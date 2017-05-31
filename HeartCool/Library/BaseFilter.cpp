#include "BaseFilter.h"

BaseFilter::BaseFilter(){
	params = 0;
	len = 0;
}
BaseFilter::BaseFilter(double *params, int len){
	this->params = params;
	this->len = len;
}

double BaseFilter::filter(double in){
	return in;
}

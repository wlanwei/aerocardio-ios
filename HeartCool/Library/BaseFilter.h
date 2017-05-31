#ifndef _BASEFILTER_
#define _BASEFILTER_

class BaseFilter{
	public:
		BaseFilter();
	   	BaseFilter(double *params, int len);
	private:
		double* params;
		int len;
		double* buffer;
	public:
		double filter(double in);
		
};
#endif

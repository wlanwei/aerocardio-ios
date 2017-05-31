#ifndef _DSPENGINE_
#define _DSPENGINE_

#include <queue>
#include <iostream>
#include <thread>
#include <mutex>

#include "commons.h"
#ifdef __UNIX__
  #include <unistd.h>  // used in linus 
#else
 #include <windows.h>        // used in windows
#endif

#include "Ecg.h"
//#include "BaseFilter.h"
#include "EcgMark.h"

using namespace std;
class DspEngine{
	public:
		DspEngine();
	private:
		static const int MAX_QUEUE_IN = 1000;
		static const int MAX_QUEUE_OUT = 1000;

	private:
		queue<Ecg> input;
		mutex mut_input;

		queue<Ecg> output;
		mutex mut_output;


		queue<EcgMark> outputM;
		mutex mut_outputM;

	/*private:
		BaseFilter lpFilter;*/

	private:
		thread thrd_dsp;
		void run_dsp();
		int flg_run_dsp;

	//raw ecg get and put
	public:
		int put_ecg_raw(Ecg ecg);
	private:
		Ecg get_ecg_raw();

	private:
		int put_ecg_dsp(Ecg ecg);
	public:
		Ecg get_ecg_dsp();

	private:
		int put_ecgMark_dsp(EcgMark ecgMark);
	public:
		EcgMark get_ecgMark_dsp();

	public:
		void stop_work();

		//filtering
	/*public:
	   Ecg firFilter(Ecg ecgIn);*/
};

#endif

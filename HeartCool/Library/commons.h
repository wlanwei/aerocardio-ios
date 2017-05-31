#ifndef __COMMONS__
#define __COMMONS__

//#define __WIN__
#define __UNIX__

#ifdef __WIN__
#define P_SLEEP Sleep
#else
#define P_SLEEP usleep
#endif

//#define boolean bool
#endif

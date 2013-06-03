#include "system.h"


void waiting(int sec)
{
	#if defined(unix) || defined(__APPLE__)
	sleep(sec);
	#elif defined(WIN32) || defined(WIN64)
	Sleep(sec*1000);
	#endif
}

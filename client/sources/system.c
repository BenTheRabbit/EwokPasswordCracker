#include "system.h"


int get_core_count()
{
	
	#if defined(WIN32) || defined(WIN64)
	SYSTEM_INFO sysinfo;
	#endif

	int nb_core = 1;
	
	#if defined(unix) || defined(__APPLE__)
	nb_core = sysconf(_SC_NPROCESSORS_CONF);
	#elif defined(WIN32) || defined(WIN64)
	GetSystemInfo( &sysinfo );

	nb_core = sysinfo.dwNumberOfProcessors;
	#endif
	return nb_core;
}


void waiting(int sec)
{
	#if defined(unix) || defined(__APPLE__)
	sleep(sec);
	#elif defined(WIN32) || defined(WIN64)
	Sleep(sec*1000);
	#endif
}

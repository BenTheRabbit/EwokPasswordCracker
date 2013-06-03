/*

 Description :

 Contain funtion that differs between systems

*/

#ifndef __SYSTEM_H__
#define __SYSTEM_H__


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


#if defined(unix) || defined(__APPLE__)
	#include <unistd.h>
#elif defined(WIN32) || defined(WIN64)
	#define _WINSOCKAPI_
    #include <windows.h>
#endif


void waiting(int sec);


#endif



#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <gmp.h>

#if defined(unix) || defined(__APPLE__)
    #include <sys/socket.h>
	#include <unistd.h>
    #include <netinet/in.h>
    #include <netdb.h>
	#include <sys/types.h>
#elif defined(WIN32) || defined(WIN64)
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#endif

#include "module_md5.h"
#include "const.h"
#include "word.h"
#include "data_struct.h"
#include "test.h"
#include "config.h"
#include "socket.h"
#include "system.h"
#include "master.h"



void getConfig(char*, config*);
void calcFunction(param* args);
int getCharset(char*, char*);
void threadCompl();


#endif

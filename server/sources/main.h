#ifndef __MAIN_H__
#define __MAIN_H__


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <gmp.h>

#if defined(unix) || defined(__APPLE_)
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <sys/types.h>
#elif defined(WIN32) || defined(WIN64)
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#endif

#include "data_struct.h"
#include "socket.h"
#include "client.h"
#include "config.h"
#include "const.h"
#include "ui.h"
#include "api_httpd.h"




#endif


#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <string.h>

#if defined(unix) || defined(__APPLE__)
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
	#include <sys/types.h>
	#include <unistd.h>
#elif defined(WIN32) || defined(WIN64)
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#endif

#include "const.h"
#include "ui.h"
#include "config.h"


void error(const char *msg);

int socket_connect(char *host, int port);
int socket_try_connect(char *host, int port);

int socket_send_mpz_t(int sockid, mpz_t nb);
int socket_receive_mpz_t(int sockid, mpz_t *nb);

int socket_send_int(int sockid, int nb);
int socket_receive_int(int sockid, int *nb);

int socket_send_string(int sockid, char *str);
int socket_receive_string(int sockid, char *str);

int socket_send_long(int sockid, long nb);
int socket_receive_long(int sockid, long *nb);


void socket_close(int sockfd);



#endif

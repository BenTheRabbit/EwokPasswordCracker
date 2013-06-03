#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <stdlib.h>
#include <stdio.h>

#if defined(unix) || defined(__APPLE__)
	#include <arpa/inet.h>
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

typedef struct
{
    int socket_id;
    long idp[HASH_TYPE_COUNT];
	long connect_time;
    int rev_num;
    queue range_cli;
	char ip[16];
	int port;
}client;



typedef struct
{
    int socketfd;
    list *client_list;
    list *hash_list;
	list *rev_list;
    queue *range_todo;
    config *conf;
}client_thread_parameter;




void client_connect(void* para);
void client_quit(list* clients, client* cli, queue* range_todo);




#endif

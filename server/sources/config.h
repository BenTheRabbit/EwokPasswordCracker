#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <pthread.h>

#if defined(unix) || defined(__APPLE__)
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <sys/types.h>
#elif defined(WIN32) || defined(WIN64)
	#include <winsock2.h>
	#pragma comment(lib, "ws2_32.lib")
#endif

#include "const.h"


typedef struct
{
    int server_port;
    int count_unit;
    int sockfd;
    int rev_num;
	int total_priority;
    long up_time; 
	mpz_t total_prior_calc_progress;
    pthread_mutex_t mutex_range;
    struct sockaddr_in serv_addr;

    
}config;

void config_get(char * file_path, config * conf);
void config_get_string(char* file_path, char* search, char* output);
int config_get_int(char* file_path, char* search);
void config_set_int(char* file_path, char* word, int value);
void config_set_string(char* file_path, char* word, char* value);

#endif

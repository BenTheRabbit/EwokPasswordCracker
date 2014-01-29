#ifndef __API_ROUTER_H__
#define __API_ROUTER_H__


#include <stdlib.h>
#include <stdio.h>
#include <jansson.h>
#include <string.h>
#include <pthread.h>

#include "api_executor.h"
#include "data_struct.h"
#include "client.h"

int api_router_url_handler(char* url, list *url_args, char* method, list *clients, list *hashes, char **resp_string, pthread_mutex_t *mutex);


#endif

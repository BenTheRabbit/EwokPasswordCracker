#ifndef __API_HTTPD_H__
#define __API_HTTPD_H__

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <microhttpd.h>

#include "data_struct.h"
#include "api_router.h"
#include "client.h"


typedef struct
{
    list* hashes;
    list* clients;
    pthread_mutex_t *mutexes;
}api_thread_parameters_t;

typedef struct
{
	char *key;
	char *value;
}api_url_arg_pair_t;

int api_httpd_daemon(void* params);
int api_httpd_get_url_args(void* cls, enum MHD_ValueKind kind, const char* key, const char* value);
int api_httpd_answer_to_connection(void *cls, struct MHD_Connection *connection,
                          const char *url, const char *method,
						  const char *version, const char *upload_data,
						  size_t *upload_data_size, void **con_cls);

#endif

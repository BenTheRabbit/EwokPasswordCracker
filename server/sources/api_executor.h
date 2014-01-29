#ifndef __API_EXECUTOR_H__
#define __API_EXECUTOR_H__

#include <stdlib.h>
#include <stdio.h>
#include <jansson.h>

#include "data_struct.h"
#include "module_md5.h"
#include "client.h"


#define API_ERROR_URL_UNKNOWN_URL_CODE -1
#define API_ERROR_URL_UNKNOWN_URL_STRING "Unknow url"
#define API_ERROR_URL_INVALID_ID_CODE -2
#define API_ERROR_URL_INVALID_ID_STRING "Invalid ID"

int api_exec_list_hashes(list* hashes, json_t *resp_root);
int api_exec_list_clients(list* clients, json_t *resp_root);


#endif


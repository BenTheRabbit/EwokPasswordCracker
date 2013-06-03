
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "system.h"
#include "ui.h"
#include "const.h"


typedef struct
{
    int nb_core;
	int nb_gpu;
    char server_address[100];
    int server_port;
    long idp[HASH_TYPE_COUNT][2];

}config;

void config_get(char * file_path, config * conf);
void config_get_string(char* file_path, char* search, char* output);
int config_get_int(char* file_path, char* search);
void config_set_int(char* file_path, char* word, int value);
void config_set_string(char* file_path, char* word, char* value);

#endif

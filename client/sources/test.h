#ifndef __TEST_H__
#define __TEST_H__


#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <gmp.h>
#include <string.h>

#include "module_md5.h"
#include "word.h"
#include "system.h"
#include "const.h"
#include "config.h"

typedef struct
{
	int * time_over;
	long * idp;
	int * mutex_running;
	pthread_mutex_t * mutex_finish_test;
	pthread_mutex_t * mutex_idp;
}param_test_thread;

typedef struct 
{
	int * time_over;
	int nb_core;
}param_time_thread;

void test_time_thread(param_time_thread * p);
void test_function_md5(param_test_thread* p);
void calculate_idp(config * conf);
void test_finish_thread(param_test_thread* p, long count);

#endif

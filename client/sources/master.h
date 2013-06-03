
#ifndef __MASTER_H__
#define __MASTER_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <gmp.h>
#include <time.h>

#include "config.h"
#include "data_struct.h"
#include "word.h"
#include "system.h"

#include "module_md5.h"


typedef struct
{
	hash_t hash;
	mpz_t start;
	mpz_t count;
	int status;

}master_range_unit;

typedef struct
{
	range_t *original_range;
	master_range_unit *ranges;
	pthread_t *pth;
}master_range;


typedef struct
{
	config *conf;
	list *hash_list;
	list *range_list;
	pthread_mutex_t *mutex;

}param_master_main;

typedef struct
{
	master_range_unit *range;
	hash_t *hash;
	int *ressource;
	list *range_list;
	pthread_mutex_t *mutex;

}param_master_calc;



void master_main(void *param);
void master_calc_cpu(void *param);
master_range* master_fragment_range(range_t *r, int nb_cpu, int nb_gpu, long idp_cpu, long idp_gpu);
void master_module_md5_bf_cpu(char* result, mpz_t start, mpz_t count, void* hash, pthread_mutex_t *mutex, int *hash_status);


#endif

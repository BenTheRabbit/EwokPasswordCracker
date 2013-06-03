#ifndef __DATA_STRUCT_H__
#define __DATA_STRUCT_H__

#include <stdio.h>
#include <stdlib.h>
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

/* Queue */

typedef struct q_link
{
    void *data;
    struct q_link *next;
}queue_link;

typedef struct
{
    queue_link *first;
    queue_link *last;
}queue;



void queue_init(queue *q);
void queue_enqueue(queue *q, void* data);
void* queue_dequeue(queue *q);
int queue_isempty(queue *q);




/* Table */

typedef struct l_link
{
    struct l_link *next;
    void *data;
}list_link;

typedef struct
{
    int size;
    list_link *first;
}list;

void list_init(list *l);
void* list_get(list *l, int n);
void list_add(list *l, void *data);
void list_remove(list *l, int n);
int list_isempty(list *l);
int list_get_size(list *l);



typedef struct
{
    mpz_t start;
    mpz_t nb_op;
    int id_hash;
}range_t;


typedef struct
{
    int id;
    int type;
	int priority;
	long start_time;
	long end_time;
	char clear_password[100];
	int status;
	mpz_t prior_calc_progress;
	void* hash;
    mpz_t next_calc;
    pthread_mutex_t mutex_next;
}hash_t;

typedef struct
{
	int hash_id;
	int rev_num;
	int type;
}revision_item;



#endif

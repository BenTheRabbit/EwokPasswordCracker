#ifndef __DATA_STRUCT_H__
#define __DATA_STRUCT_H__

#include <stdlib.h>
#include <stdio.h>


#include <gmp.h>
#include <time.h>


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




/* List */

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
    mpz_t count;
    int nb_char;
    mpz_t *found;
    char* charset;
    char* hash;
}param;


typedef struct
{
	int type;
	int id;
	char clear_password[100];
	int status;
	void* hash;
} hash_t;


typedef struct
{
	int type;
	int hash_id;
	hash_t *hash;
	int status;
	time_t start_time;
	time_t end_time;
	mpz_t start;
	mpz_t count;
	void* custom_range;
} range_t;



#endif

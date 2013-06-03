#ifndef __UI_H__
#define __UI_H__

#include <ncurses.h>
#include <gmp.h>
#include <string.h>
#include <form.h>
#include <pthread.h>

#include "system.h"
#include "config.h"
#include "const.h"
#include "data_struct.h"
#include "client.h"
#include "module_md5.h"
#include "word.h"


typedef struct
{
    config *conf;
    list* hashes;
    list* clients;
    pthread_mutex_t *mutex;
}ui_thread_parameter;



void ui_thread(void* param);
void ui_init();
void ui_display_server_info(WINDOW *win, int x, int y, config *conf);
void ui_format_time(long number, char *texte);
void ui_display_hash(WINDOW *win, int x, int y, list* hash);
void ui_display_client(WINDOW *win, int x, int y, list* client);
void ui_get_int(char * question, int * output, int limit_min, int limit_max, int window_size_x, int window_size_y, int default_value);


#endif

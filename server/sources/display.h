#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <ncurses.h>

#include "client.h"
#include "data_struct.h"

typedef struct
{
    list *client_list;
    list *hash_list;
    config *srv_conf;
} paramDisplayThread;

void initDisplay();
void* displayThread(void* tmp_param);


#endif


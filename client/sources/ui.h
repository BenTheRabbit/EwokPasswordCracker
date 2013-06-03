

#ifndef __UI_H__
#define __UI_H__


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "const.h"

int ui_input_int(char* question, int default_value, int min_int, int max_int);
int ui_input_string(char* question, char* output, char* default_value);

char ui_multi_choice_question(char* question, char* possible, char default_value);


#endif

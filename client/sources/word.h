
#ifndef __WORD_H__
#define __WORD_H__

#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <math.h>
#include <string.h>

void nextword(char*, int, char*);
char* int2word(mpz_t, char*, int, char*);

#endif

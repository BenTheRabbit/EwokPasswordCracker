#ifndef __MODULE_MD5_H__
#define __MODULE_MD5_H__


#include "socket.h"
#include "data_struct.h"
#include "const.h"
#include "module_md5.h"


typedef struct {
	char hash[33];
	char charset[500];
} hash_md5;
	  
typedef struct {
	mpz_t start;
	mpz_t count;
} range_md5;


int module_md5_send_hash(int sockfd, hash_t* h);

#endif

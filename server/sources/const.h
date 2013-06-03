#ifndef __CONST_H__
#define __CONST_H__

#define CODE_OK 1
#define CODE_GET_NEW_RANGE 2
#define CODE_CHANGE_HASH 3
#define CODE_FOUND 4
#define CODE_QUIT 5
#define CODE_RANGE_DONE 6
#define CODE_NO_HASH 7
#define CODE_ADD_HASH 8
#define CODE_DEL_HASH 9

#define HASH_TYPE_MD5 0

#define HASH_TYPE_COUNT 1

#define HASH_STATUS_NOT_FOUND 0
#define HASH_STATUS_FOUND 1

#define BUFFER_SIZE 1024

#define TEST_TIME 5
#define TARGET_TIME 20

#define IDP_TYPE_CPU 0
#define IDP_TYPE_GPU 1

#define CONF_FILE_NAME "server.conf"

#define RANGE_STATUS_PENDING 0
#define RANGE_STATUS_RUNNING 1
#define RANGE_STATUS_FINNISH 2

#define VERSION "O.2"

#define MUTEX_COUNT 1
#define MUTEX_PORT 0

#define TAB_HASH 0
#define TAB_CLIENT 1

#endif


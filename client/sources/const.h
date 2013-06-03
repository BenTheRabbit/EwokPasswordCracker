
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
#define HASH_STATUS_SENT 2

#define BUFFER_SIZE 1024

#define TEST_TIME 5
#define TARGET_TIME 20

#define IDP_TYPE_CPU 0
#define IDP_TYPE_GPU 1

#define CONF_FILE_NAME "client.conf"

#define RANGE_STATUS_PENDING 0
#define RANGE_STATUS_RUNNING 1
#define RANGE_STATUS_FINISH 2
#define RANGE_STATUS_DELETED 3

#define MUTEX_COUNT 5

#define MUTEX_RANGE_LIST 0
#define MUTEX_HASH_LIST 1
#define MUTEX_CPU_FREE 2
#define MUTEX_GPU_FREE 3
#define MUTEX_RESSOURCE 4

#endif


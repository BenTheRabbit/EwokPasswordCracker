#include "main.h"

pthread_mutex_t mutex_end=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_mut=PTHREAD_MUTEX_INITIALIZER;
int mutex_count;

int main(int argc, char *argv[])
{
	
	/**********************/
	/** Define variables **/
	/**********************/

    #if defined(WIN32) || defined(WIN64)
        WSADATA WSAData;
        int erreur=WSAStartup(MAKEWORD(2,2),&WSAData);
    #endif
	
	//Conf file related

    config conf;
	int core_tmp;
    char result_recalculate;
	


	//Socket related

	int sockfd;

	int n;
	int socket_code_return;
	int socket_hash_type;
	int socket_change_type;
	int socket_hash_id;
	int socket_nb_change;


	//Hash list and range queue

	list hash_list;
	list range_list;
	range_t *tmp_range = NULL;
	hash_t *tmp_hash = NULL;

	//Master thread
	pthread_t pth_master;
	param_master_main param_master;

	pthread_mutex_t mutex[MUTEX_COUNT];

    int i, j;

	/**************************/
	/** Initialize variables **/
	/**************************/

	//List and queue related

	list_init(&hash_list);
	list_init(&range_list);

	for(i = 0; i < MUTEX_COUNT; i++)
	{
		pthread_mutex_init(&mutex[i], NULL);
	}
	



	/**********************/
	/** Configure client **/
	/**********************/

	core_tmp = config_get_int(CONF_FILE_NAME, "NB_CORE");
    config_get(CONF_FILE_NAME, &conf);
    
    if(conf.nb_core == core_tmp)
    {
        result_recalculate = ui_multi_choice_question("Do you want to recalculate your performance indicator ?", "yn", 'n');
        if(result_recalculate == 'y')
            calculate_idp(&conf);
    }
    else
    {
        calculate_idp(&conf);
    }
    

	conf.nb_gpu = 0;

	printf("\nYour idp is \t%ld\n\t\t%ld per core\n", conf.idp[HASH_TYPE_MD5][IDP_TYPE_CPU], conf.idp[HASH_TYPE_MD5][IDP_TYPE_CPU]/conf.nb_core);



    
	/******************************/
	/** Try connecting to server **/
	/******************************/

	sockfd = socket_connect(conf.server_address, conf.server_port);

	/** Update config if change **/

	config_get_string(CONF_FILE_NAME, "SERVER_ADDRESS", conf.server_address);
	conf.server_port = config_get_int(CONF_FILE_NAME, "SERVER_PORT");



    /** Send idp **/

	socket_send_long(sockfd, conf.idp[HASH_TYPE_MD5][IDP_TYPE_CPU]);


	/**************************/
	/** Launch master thread **/
	/**************************/

	
	param_master.conf = &conf;
	param_master.hash_list = &hash_list;
	param_master.range_list = &range_list;
	param_master.mutex = mutex;

	pthread_create(&pth_master, NULL, (void * (*)(void *))master_main, (void*)&param_master);

    while(1)
    {
		/*********************/
		/** Send range done **/
		/*********************/

		for(i = 0; i < list_get_size((void*)&range_list); i++)
		{
			tmp_range = (range_t*)list_get((void*)&range_list, i);
			if(tmp_range->status == RANGE_STATUS_FINISH)
			{
				//Send code range done

				socket_send_int(sockfd, CODE_RANGE_DONE);
				// Send compute time

				socket_send_int(sockfd, (int)(tmp_range->end_time - tmp_range->start_time));

				pthread_mutex_lock(&mutex[MUTEX_RANGE_LIST]);
				list_remove((void*)&range_list, i);
				pthread_mutex_unlock(&mutex[MUTEX_RANGE_LIST]);
			}
			else
			{
				break;
			}
		}


		/********************/
		/** Send key found **/
		/********************/

		for(i = 0; i < list_get_size((void*)&hash_list); i++)
		{
			tmp_hash = list_get((void*)&hash_list, i);
			if(tmp_hash->status == HASH_STATUS_FOUND)
			{

				socket_send_int(sockfd, CODE_FOUND);
				socket_send_int(sockfd, tmp_hash->id);
				socket_send_string(sockfd, tmp_hash->clear_password);
				tmp_hash->status = HASH_STATUS_SENT;
			}
		}


		/****************/
		/** Get ranges **/
		/****************/
		tmp_range = (range_t*)list_get(&range_list, list_get_size(&range_list)-1);
		if(list_isempty(&range_list) || tmp_range->status > RANGE_STATUS_PENDING)
		{
		printf("Need new range!!!\n");
			socket_send_int(sockfd, CODE_GET_NEW_RANGE);
	
			socket_receive_int(sockfd, &socket_code_return);
			
			switch(socket_code_return)
			{
				case CODE_OK:
					tmp_range = (range_t*)malloc(sizeof(range_t));

					mpz_init(tmp_range->start);
					mpz_init(tmp_range->count);

					socket_receive_int(sockfd, &(tmp_range->type));
					socket_receive_int(sockfd, &(tmp_range->hash_id));
					tmp_range->status = RANGE_STATUS_PENDING;
					socket_receive_mpz_t(sockfd, &(tmp_range->start));
					socket_receive_mpz_t(sockfd, &(tmp_range->count));
					
					for(i = 0; i < list_get_size(&hash_list); i++)
					{
						tmp_hash = (hash_t*)list_get(&hash_list, i);
						if(tmp_hash->id == tmp_range->hash_id)
						{
							break;
						}
					}

					tmp_range->hash = tmp_hash;
					
					/** TODO **/
					// Use mutex to block master thread

					pthread_mutex_lock(&mutex[MUTEX_RANGE_LIST]);

					list_add(&range_list, tmp_range);

					pthread_mutex_unlock(&mutex[MUTEX_RANGE_LIST]);

					break;

				case CODE_CHANGE_HASH:
					

					socket_receive_int(sockfd, &socket_nb_change);
					
					for(j = 0; j < socket_nb_change; j++)
					{
						socket_receive_int(sockfd, &socket_change_type);
						if(socket_change_type == CODE_DEL_HASH)
						{
							socket_receive_int(sockfd, &socket_hash_id);

							// Set related range status to deleted

							pthread_mutex_lock(&mutex[MUTEX_RANGE_LIST]);
							for(i = 0; i < list_get_size((void*)&range_list); i++)
							{
								tmp_range = (range_t*)list_get((void*)&range_list, i);
								if(tmp_range->hash_id == socket_hash_id)
								{
									if(tmp_range->status == RANGE_STATUS_PENDING)
									{
										tmp_range->start_time = 0;
										tmp_range->end_time = 20;
										tmp_range->status = RANGE_STATUS_FINISH;	
									}
									else
									{
										tmp_range->status = RANGE_STATUS_DELETED; 
									}
								}
							}

							pthread_mutex_unlock(&mutex[MUTEX_RANGE_LIST]);
							

							// Delete hash

							pthread_mutex_lock(&mutex[MUTEX_HASH_LIST]);
							for(i = 0; i < list_get_size((void*)&hash_list); i++)
							{
								tmp_hash = (range_t*)list_get((void*)&hash_list, i);
								if(tmp_hash->id == socket_hash_id)
								{
									tmp_hash->status = HASH_STATUS_FOUND;
									waiting(1);
									list_remove((void*)&hash_list, i);
								}
							}

							pthread_mutex_unlock(&mutex[MUTEX_HASH_LIST]);



						}
						else
						{
								socket_receive_int(sockfd, &socket_hash_type);
								switch(socket_hash_type)
								{
									case HASH_TYPE_MD5:
										module_md5_receive_hash(sockfd, &hash_list);
										break;
								}
						}
					}

					break;

				case CODE_NO_HASH:
					printf("\nNo hash available\n");
					break;

			}

		}


		waiting(2);

    }

	/*****************/
	/** Free memory **/
	/*****************/


    #if defined(WIN32) || defined(WIN64)
        WSACleanup();
    #endif
    socket_close(sockfd);
    return 0;
}



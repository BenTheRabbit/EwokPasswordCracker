#include "main.h"

int main(int argc, char *argv[])
{
    
    /*****************/
    /** Declaration **/
    /*****************/
    int i;
    socklen_t clilen;
    pthread_t pth_client = 0;
    pthread_t pth_ui = 0;
    pthread_mutex_t mutex[MUTEX_COUNT];
    
    
	client_thread_parameter *param_sock;
	ui_thread_parameter param_ui;
    
    struct sockaddr_in cli_addr;
    
    config srv_conf;
    
    list clients;
    list hashs;
	list revision;
    
	/*******************/
	/* Initialization **/
	/*******************/
    
    
    list_init(&clients);
    list_init(&hashs);
	list_init(&revision);
    
    
    for(i=0; i<MUTEX_COUNT;i++)
    {
        pthread_mutex_init(&mutex[i], NULL);
    }
    
    srv_conf.up_time = time(NULL);
    
	hash_md5 tt;
	strcpy(tt.hash,"bc11f06afb9b27070673471a23ecc6a9");
	strcpy(tt.charset, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@!*-$#^?");
	
	hash_t test;
	test.id = 1;
	test.priority = 5;
	test.type = HASH_TYPE_MD5;
	test.status = HASH_STATUS_NOT_FOUND;
	test.hash = (void*)(&tt);
	mpz_init(test.prior_calc_progress);
	mpz_init(test.next_calc);
	test.start_time = time(NULL);
    
    
	hash_t test2;
	test2.id = 2;
	test2.priority = 1;
	test2.type = HASH_TYPE_MD5;
	test2.status = HASH_STATUS_NOT_FOUND;
	test2.hash = (void*)(&tt);
	mpz_init(test2.prior_calc_progress);
	mpz_init(test2.next_calc);
	test2.start_time = time(NULL);
    
    
	
	revision_item rev1, rev2;
    
	rev1.hash_id = 1;
	rev1.type = CODE_ADD_HASH;
	rev2.hash_id = 2;
	rev2.type = CODE_ADD_HASH;
    
	list_add((void*)&revision, (void*)&rev1);
	//list_add((void*)&revision, (void*)&rev2);
    
    
	list_add(&hashs, (void*)(&test));
	//list_add(&hashs, (void*)(&test2));
	srv_conf.rev_num = 1;
	srv_conf.total_priority = 5; // TODO set to 0;
	mpz_init(srv_conf.total_prior_calc_progress);
    
    queue range_todo;
    queue_init(&range_todo);
    
    
    
    /*****************/
    /** Server init **/
    /*****************/
    
    config_get(CONF_FILE_NAME, &srv_conf);
    
    /****************/
    /** Launch GUI **/
    /****************/
    
    param_ui.hashes = &hashs;
    param_ui.clients = &clients;
    param_ui.conf = &srv_conf;
    param_ui.mutex = mutex;
    
    pthread_mutex_lock(&mutex[MUTEX_PORT]);
   	pthread_create(&pth_ui, 0, (void * (*)(void*))&ui_thread, (void*)&param_ui);
    pthread_detach(pth_ui);
    pthread_mutex_lock(&mutex[MUTEX_PORT]);

    pthread_mutex_unlock(&mutex[MUTEX_PORT]);
    
    /*****************/
    /** Socket init **/
    /*****************/
    
    
    socket_init(&srv_conf);
    clilen = sizeof(cli_addr);
    
    
    

    
    
    /****************************/
    /** Connection client loop **/
    /****************************/
    
    while(1)
    {
        param_sock = (client_thread_parameter*)malloc(sizeof(client_thread_parameter));
		param_sock->client_list = &clients;
		param_sock->hash_list = &hashs;
		param_sock->range_todo = &range_todo;
		param_sock->rev_list = &revision;
		param_sock->conf = &srv_conf;
        
        if((param_sock->socketfd = accept(srv_conf.sockfd,(struct sockaddr *) &cli_addr, &clilen)) != -1)
        {
            pthread_create(&pth_client, 0, (void*(*)(void*))&client_connect, (void*)param_sock);
            pthread_detach(pth_client);
        }
        else
        {
            printf("ERROR on accept client");
        }
        
    }
    
    
    /*******************/
    /** Cleaning step **/
    /*******************/
    
    socket_close(srv_conf.sockfd);
    return 0;
}



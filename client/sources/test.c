#include "test.h"

void calculate_idp(config * conf)
{
    
	/**********************/
	/** Define variables **/
	/**********************/
    
	/** Calc thread parameter related **/
    
	param_test_thread * parameter;
    long idp;
	int mutex_running = conf->nb_core;
	pthread_mutex_t mutex_finish_test = PTHREAD_MUTEX_INITIALIZER; 
	pthread_mutex_t mutex_idp = PTHREAD_MUTEX_INITIALIZER; 
    
    
	/** Thread related **/
    
	pthread_t time_thread;
	pthread_t* calc_thread;
    
	int i, k;
    
	/** Time thread paramter related **/
    
	param_time_thread time_parameter;
    
    
    
	/********************/
	/** Initialization **/
	/** and allocation **/
	/********************/
    
    
	pthread_mutex_init(&mutex_finish_test, NULL);
	pthread_mutex_init(&mutex_idp, NULL);
    
	time_parameter.time_over = (int*)malloc(sizeof(int)*conf->nb_core);
	parameter = malloc(sizeof(param_test_thread)*conf->nb_core);
	calc_thread = (pthread_t*)malloc(sizeof(pthread_t)*conf->nb_core);
	
	time_parameter.nb_core = conf->nb_core;
	
	/** Parameter initialization **/
    
    for (k=0; k<HASH_TYPE_COUNT; k++) {
        
        idp=0;
        
        for(i = 0; i < conf->nb_core; i++)
        {
            time_parameter.time_over[i] = 0;
            parameter[i].time_over = &(time_parameter.time_over[i]);
            
            parameter[i].mutex_running = &mutex_running;
            parameter[i].idp = &idp;
            parameter[i].mutex_finish_test = &mutex_finish_test;
            parameter[i].mutex_idp = &mutex_idp;
        }
        
        pthread_mutex_lock(&mutex_finish_test);
        
        switch (k) {
            case HASH_TYPE_MD5:
                printf("\nCalculating MD5...\n");
                break;
        }
        
        /** Launch threads **/
        
        for(i=0; i<conf->nb_core; i++)
        {
            switch (k) {
                case HASH_TYPE_MD5:
                    pthread_create(&calc_thread[i], NULL, (void*(*)(void*))test_function_md5, (void*) &parameter[i]);
                    break;
            }
        }
        
        pthread_create(&time_thread, NULL, (void*(*)(void*))test_time_thread, &time_parameter);
        
        /** Wait for end threads **/
        pthread_mutex_lock(&mutex_finish_test);
        pthread_mutex_unlock(&mutex_finish_test);
        
        conf->idp[k][0]=(idp/TEST_TIME);
		config_set_int(CONF_FILE_NAME, "IDP_MD5_CPU", conf->idp[k][0]);
    }
}

void test_function_md5(param_test_thread* p)
{
    
    /**********************/
    /** Define variables **/
    /**********************/
    
    long count = 0;
    mpz_t nb;
    char word[100];
    int i = 0;
    
    unsigned char hash[16];
    char tmp[2];
    char test[] =  "05b28d17a7b6e7024b6e5d8cc43a8bf7";

    /**************************/
    /** Initialize variables **/
    /**************************/
    mpz_init_set_ui(nb, 540504354034);
    int2word(nb, "abcdefghijklmnopqrstuvwxyz", 26, word);
    
    /** Transfor hash (to externalize **/
    for(i = 0; i < 16; i++)
    {
        tmp[0] = test[i*2];
        tmp[1] = test[i*2+1];
        sscanf(tmp, "%02x", (unsigned int *)&hash[i]);
    }
    /** Calculate while time is not over **/
    while(*p->time_over == 0)
    {
        count++;
        
        nextword("abcdefghijklmnopqrstuvwxyz", 26, word);
        //	comp(hash, (unsigned char*)md5(word));
        module_md5_comp(hash, word);
    }
    /** When finish, block to avoid conflict on idp and mutex_running**/
    
    test_finish_thread(p, count);
    /*pthread_mutex_lock(p->mutex_idp);
    
    *p->mutex_running -= 1;
    *p->idp += count;
    
     If mutex_runing reach 0, all thread are terminated. End program
    if(*p->mutex_running <= 0)
    {
        pthread_mutex_unlock(p->mutex_finish_test);	
    }
    
    pthread_mutex_unlock(p->mutex_idp);
     */
    
}

void test_time_thread(param_time_thread * p)
{
    int i; 
    
    waiting(TEST_TIME);
    for(i = 0; i < p->nb_core; i++)
    {
        p->time_over[i] = 1;
    }
    
}

void test_finish_thread(param_test_thread* p, long count)
{
    pthread_mutex_lock(p->mutex_idp);
    
    *p->mutex_running -= 1;
    *p->idp += count;
    
    /** If mutex_runing reach 0, all thread are terminated. End program **/
    if(*p->mutex_running <= 0)
    {
        pthread_mutex_unlock(p->mutex_finish_test);	
    }
    
    pthread_mutex_unlock(p->mutex_idp);
}



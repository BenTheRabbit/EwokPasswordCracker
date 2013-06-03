#include "master.h"


void master_main(void *param)
{

	/***********************/
	/** Declare variables **/
	/***********************/

	param_master_main *p = (param_master_main*)param;

	int gpu_free = p->conf->nb_gpu;
	int cpu_free = p->conf->nb_core;
	int i = 0, j = 0, boolean = 0;

	list range_fragmented; 

	range_t *tmp_range;
	master_range *tmp_range_frag;

	param_master_calc *param_calc;

	/**********/
	/** Init **/
	/**********/

	list_init(&range_fragmented);



	while(1)
	{
	//printf("\nWhile 1\n");

		//While some ressources are free
		while(cpu_free > 0 || gpu_free > 0)
		{


				//printf("\nWhile ressource\n");
			//Select ressource to launch

			//If GPU
			if(gpu_free > 0)
			{
				//In fragmented hash list, look if one of 
				//the reseved GPU range is free (running or finnish)

		/*		for()
				{

				}

				//If not found
				if()
				{
					//Fragment a new one from normal list
				}

				//At this moment a range is selected
				//Try lock mutex
				//Launch the thread
*/

			}
			//If CPU
			else
			{
				



				/********************/
				/** Select a range **/
				/********************/

				//In fragmented hash list, look if one of 
				//the reseved CPU range is free (running or finnish)

				tmp_range_frag = NULL;

				for(i = 0; i < list_get_size(&range_fragmented); i++)
				{
					tmp_range_frag = (master_range*)list_get(&range_fragmented, i);
					
					for(j = p->conf->nb_gpu; j < p->conf->nb_gpu + p->conf->nb_core; j++)
					{
						if((tmp_range_frag->ranges[j].status == RANGE_STATUS_PENDING) || j == (p->conf->nb_gpu + p->conf->nb_core - 1))
						{
							break;
						}
					}
				}



				/************************/
				/** If no available    **/
				/** fragment a new one **/
				/************************/

				if((list_isempty(&range_fragmented)) || (tmp_range_frag->ranges[j].status != RANGE_STATUS_PENDING))
				{
					tmp_range = NULL;
					tmp_range_frag = NULL;

					//Select a new range
					for(i = 0; i < list_get_size(p->range_list); i++)
					{
						tmp_range = (range_t*)list_get(p->range_list, i);
						if(tmp_range->status == RANGE_STATUS_PENDING)
						{
							break;
						}
					}

					//Fragment a new one from normal list
					if(tmp_range != NULL && tmp_range->status == RANGE_STATUS_PENDING)
					{
						tmp_range_frag = master_fragment_range(tmp_range, p->conf->nb_core, p->conf->nb_gpu, p->conf->idp[tmp_range->type][IDP_TYPE_CPU], p->conf->idp[tmp_range->type][IDP_TYPE_GPU]);
	
						list_add(&range_fragmented, tmp_range_frag);
		
						j = p->conf->nb_gpu;
					}
				}




				//At this moment a range is selected
				//Try lock mutex
				//Launch the thread

				if(tmp_range_frag != NULL)
				{
					param_calc = (param_master_calc*)malloc(sizeof(param_master_calc));
					param_calc->range = &tmp_range_frag->ranges[j];
					param_calc->hash = tmp_range_frag->original_range->hash;
					param_calc->ressource = &cpu_free;
					param_calc->mutex = p->mutex;
					param_calc->range_list = p->range_list;
					param_calc->range->status = RANGE_STATUS_RUNNING;
					tmp_range_frag->original_range->start_time = time(NULL);

					pthread_mutex_trylock(&p->mutex[MUTEX_RESSOURCE]);

					pthread_create(&tmp_range_frag->pth[j], NULL, (void * (*)(void *))master_calc_cpu, (void*)param_calc);

					pthread_mutex_lock(&p->mutex[MUTEX_CPU_FREE]);	
					cpu_free--;
					pthread_mutex_unlock(&p->mutex[MUTEX_CPU_FREE]);	
				}
				else
				{
					waiting(1);
				}

			}


		}
		
		// Update range status

		for(i = 0; i < list_get_size((void*)&range_fragmented); i++)
		{
			boolean = 0;
			tmp_range_frag = (master_range*)list_get((void*)&range_fragmented, i);
			for(j = 0; j < (p->conf->nb_core + p->conf->nb_gpu); j++)
			{
				if(tmp_range_frag->ranges[j].status != RANGE_STATUS_FINISH)
				{
					boolean = 1;
					break;
				}
			}
			if(boolean == 0)
			{
				pthread_mutex_lock(&p->mutex[MUTEX_RANGE_LIST]);	
				
				tmp_range_frag->original_range->status = RANGE_STATUS_FINISH;
				tmp_range_frag->original_range->end_time = time(NULL);
				list_remove((void*)&range_fragmented, i);

				pthread_mutex_unlock(&p->mutex[MUTEX_RANGE_LIST]);	
			}

		}

printf("\n----------------------------------\n");
		//Block until a ressource get free

		pthread_mutex_lock(&p->mutex[MUTEX_RESSOURCE]);
	}

	pthread_exit(NULL);

}


void master_calc_cpu(void *param)
{
	param_master_calc *p = (param_master_calc*)param;
	char result[100];
	range_t *tmp_range;
	int i;



	result[0] = -1;

	
	switch(p->hash->type)
	{
		case HASH_TYPE_MD5:
			
			master_module_md5_bf_cpu(result, p->range->start, p->range->count, p->hash->hash, p->mutex, &p->hash->status);
			break;
	}

	if(result[0] != -1)
	{

		pthread_mutex_lock(&p->mutex[MUTEX_RANGE_LIST]);
		for(i = 0; i < list_get_size((void*)p->range_list); i++)
		{
			tmp_range = (range_t*)list_get((void*)p->range_list, i);
			if(tmp_range->hash_id == p->hash->id)
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
		pthread_mutex_unlock(&p->mutex[MUTEX_RANGE_LIST]);							




		strcpy(p->hash->clear_password, result);
		printf("\nFound : %s\n", p->hash->clear_password);
		p->hash->status = HASH_STATUS_FOUND;
	}

	p->range->status = RANGE_STATUS_FINISH;	

	pthread_mutex_lock(&p->mutex[MUTEX_CPU_FREE]);	
	*p->ressource = *p->ressource + 1;
	pthread_mutex_unlock(&p->mutex[MUTEX_RESSOURCE]);	

	pthread_mutex_unlock(&p->mutex[MUTEX_CPU_FREE]);	
	
	pthread_exit(NULL);
}



master_range* master_fragment_range(range_t *r, int nb_cpu, int nb_gpu, long idp_cpu, long idp_gpu)
{

	master_range *tmp_range;
	master_range_unit cpu_range;
	master_range_unit gpu_range;
	long ratio_cpu;
	int i;

	mpz_t rest;
	mpz_t nb_op;

	/** Initialization **/

	tmp_range = (master_range*)malloc(sizeof(master_range));
	tmp_range->ranges = (master_range_unit*)malloc(sizeof(master_range_unit)*(nb_cpu+nb_gpu));
	tmp_range->pth = (pthread_t*)malloc(sizeof(pthread_t)*(nb_cpu+nb_gpu));

	tmp_range->original_range = r;

	mpz_init(cpu_range.start);
	mpz_init(cpu_range.count);
	mpz_init(gpu_range.start);
	mpz_init(gpu_range.count);
	mpz_init(rest);
	mpz_init(nb_op);


	for(i = 0; i < nb_cpu + nb_gpu; i++)
	{
		mpz_init(tmp_range->ranges[i].start);
		mpz_init(tmp_range->ranges[i].count);
		tmp_range->ranges[i].status = RANGE_STATUS_PENDING;
	}


	ratio_cpu = (idp_cpu*100)/(idp_cpu+idp_gpu);

	if(nb_gpu>0)
	{
		//TODO
	}
	else
	{
		mpz_set(cpu_range.start, r->start);
		mpz_set(cpu_range.count, r->count);
	}

	//Fill gpu ranges
	if(nb_gpu > 0)
	{
		//TODO
	}

	//Fill cpu ranges

	//gmp_printf("\nRange : %Zd, %Zd\n", r->start, r->count);
	mpz_cdiv_qr_ui(nb_op, rest, cpu_range.count, nb_cpu);
	for(i = nb_gpu; i < nb_cpu+nb_gpu; i++)
	{
		//Set start
		mpz_mul_ui(tmp_range->ranges[i].start, nb_op, i);
		mpz_add(tmp_range->ranges[i].start, tmp_range->ranges[i].start, r->start);



		//Set count
		mpz_set(tmp_range->ranges[i].count, nb_op);
		if(i == (nb_cpu + nb_gpu - 1))
		{
			mpz_add(tmp_range->ranges[i].count, tmp_range->ranges[i].count, rest);
		}


		//gmp_printf("\nStart : %Zd\nCount : %Zd\nStatus : %d", tmp_range->ranges[i].start, tmp_range->ranges[i].count, tmp_range->ranges[i].status);
	}


	r->status = RANGE_STATUS_RUNNING;


	return tmp_range;
}


void master_module_md5_bf_cpu(char* result, mpz_t start, mpz_t count, void* hash, pthread_mutex_t *mutex, int *hash_status)
{
	// Declaration
	char *tmp_charset;
	char *tmp_hash;
	mpz_t tmp_mpz;
	unsigned char digest[16];
	char tmp[2];
	hash_md5 *custom_hash;
	int i;
	char word[100];
	unsigned long long tmp_int64 = 0;
	unsigned long long count_int64;

	// Initialization



	mpz_init_set_ui(tmp_mpz, 0);

	count_int64 = mpz_get_ui(count);
	pthread_mutex_lock(&mutex[MUTEX_HASH_LIST]);

	custom_hash = (hash_md5*)hash;

	tmp_charset = (char*)malloc(sizeof(char)*(strlen(custom_hash->charset)+1));
	tmp_hash = (char*)malloc(sizeof(char)*(strlen(custom_hash->hash)+1));

	strcpy(tmp_charset, custom_hash->charset);
	strcpy(tmp_hash, custom_hash->hash);


	tmp_charset[strlen(custom_hash->charset)] = 0;
	tmp_hash[strlen(custom_hash->hash)] = 0;

	pthread_mutex_unlock(&mutex[MUTEX_HASH_LIST]);

	// Transform hash
	for(i = 0; i < 16; i++)
    {
        tmp[0] = tmp_hash[i*2];
        tmp[1] = tmp_hash[i*2+1];
        
		digest[i] = strtol(tmp, NULL, 16);
    }
	
	int2word(start, tmp_charset, strlen(tmp_charset), word);
	//for(mpz_set_ui(tmp_mpz, 0); mpz_cmp(count, tmp_mpz) >= 0; mpz_add_ui(tmp_mpz, tmp_mpz, 1))
	for(tmp_int64 = 0; tmp_int64 <= count_int64 ; ++tmp_int64)
	{
		if(*hash_status == HASH_STATUS_FOUND)
		{
			break;
		}
		if(module_md5_comp(digest, word) == 1)
		{
			strcpy(result, word);
			break;
		}
		nextword(tmp_charset, strlen(tmp_charset), word);
	}


	free(tmp_charset);
	free(tmp_hash);
	mpz_clear(tmp_mpz);
}






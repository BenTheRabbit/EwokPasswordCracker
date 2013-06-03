#include "client.h"




void client_connect(void* para)
{
    client_thread_parameter *p = (client_thread_parameter*)para;

    int sockfd = p->socketfd;
    int n;
    int i, j;
	int code;
	int tmp_return;
    hash_t* tmp_hash;
	range_t* tmp_range;
	revision_item *tmp_rev_item;
	
    client cli;

	mpz_t target_ratio;
	mpz_t actual_ratio;

	struct sockaddr_in address;
	socklen_t address_size;

	address_size = sizeof(struct sockaddr_in);
	
	
	n = getpeername(sockfd, (struct sockaddr*)&address, &address_size);
	/*if(n<=0)
	{
		return;
	}*/



    /** Read idp **/

	n = socket_receive_long(sockfd, &(cli.idp[HASH_TYPE_MD5]));
	if(n<=0)
	{
		return;
	}

    cli.socket_id = sockfd;
    cli.rev_num = 0;
	cli.connect_time = time(NULL);
	queue_init(&cli.range_cli);

	cli.port = ntohs(address.sin_port);
	strcpy(cli.ip, inet_ntoa(address.sin_addr));

	mpz_init(actual_ratio);
	mpz_init(target_ratio);
    list_add(p->client_list, (void*)&cli);

	printf("Client %s:%d connect with idp %ld\n", cli.ip, cli.port, cli.idp[HASH_TYPE_MD5]);

    code = 0;

    while(1)
    {
        /** Read code **/

        n = socket_receive_int(sockfd, &code);
		if(n<=0)
		{
			client_quit(p->client_list, &cli, p->range_todo);
			return;
		}
        /** Choose action **/

		switch(code)
		{
			case CODE_GET_NEW_RANGE:

				if(list_isempty(p->hash_list))
            	{
            	    n = socket_send_int(sockfd, CODE_NO_HASH);
		    	 	break;
            	}
            	else if(cli.rev_num != p->conf->rev_num)
            	{
		    		n = socket_send_int(sockfd, CODE_CHANGE_HASH);
					
					tmp_return = p->conf->rev_num - cli.rev_num;
		    		n = socket_send_int(sockfd, tmp_return);
            	    
		    		for(i = cli.rev_num; i<tmp_return; i++)
            	    {
						tmp_rev_item = (revision_item*)list_get((void*)p->rev_list, i);

						if(tmp_rev_item->type == CODE_DEL_HASH)
						{
							socket_send_int(sockfd, CODE_DEL_HASH);
							socket_send_int(sockfd, tmp_rev_item->hash_id);
						}
						else
						{
	
							socket_send_int(sockfd, CODE_ADD_HASH);
							for(j=0; j < list_get_size((void*)p->hash_list); j++)
							{
								tmp_hash = (hash_t*)list_get(p->hash_list, i);
			    	 				
								if(tmp_hash->id == tmp_rev_item->hash_id)
								{
			    	 				switch(tmp_hash->type)
			    	 				{
			    	 					case HASH_TYPE_MD5:
			    	 						n = module_md5_send_hash(sockfd, tmp_hash);
			    	 						break;
	
				    	 			}
									break;
								}
							}
						}

						cli.rev_num++;

					}
				}
            	else
            	{
            	    /** Look if there are range not done **/

					if(!queue_isempty((void*)p->range_todo))
					{
						tmp_range = (range_t*)queue_dequeue((void*)p->range_todo);
						
						// Range sent a the end
					}
					else
					{
		    	 	/** If not, choose which hash to calculate (based on priority) **/
			

						for(i = 0; i < list_get_size((void*)p->hash_list); i++)
						{
							tmp_hash = (hash_t*)list_get((void*)p->hash_list, i);
							
							// If hash still not found
							if(tmp_hash->status == HASH_STATUS_NOT_FOUND)
							{
								// Calculate target ratio
								mpz_set_ui(target_ratio, (tmp_hash->priority * 100) / p->conf->total_priority);
								

								// Calculate actual ratio
								mpz_mul_si(actual_ratio, tmp_hash->prior_calc_progress, 100);
								
								// Prevent zero division
								if(mpz_cmp_ui(p->conf->total_prior_calc_progress, 0) <= 0)
								{
									break;
								}

								mpz_cdiv_q(actual_ratio, actual_ratio, p->conf->total_prior_calc_progress);
								// END Calculate actual ratio

								// If target ratio is not reach, use this hash
								if(mpz_cmp(target_ratio, actual_ratio) >= 0)
								{
									break;
								}
							}
							else
							{
								continue;
							}
						}

		    	 	/** Finally choose range size (based on idp for this hash type) **/

						if(tmp_hash->status != HASH_STATUS_FOUND)
						{
							tmp_range = (range_t*)malloc(sizeof(range_t));

							tmp_range->id_hash = tmp_hash->id;
							mpz_init_set(tmp_range->start, tmp_hash->next_calc);
							mpz_init_set_ui(tmp_range->nb_op, cli.idp[tmp_hash->type]);
							mpz_mul_ui(tmp_range->nb_op, tmp_range->nb_op, TARGET_TIME);

							mpz_add(tmp_hash->next_calc, tmp_hash->next_calc, tmp_range->nb_op);


							mpz_add_ui(p->conf->total_prior_calc_progress, p->conf->total_prior_calc_progress, cli.idp[0]);
							mpz_add_ui(tmp_hash->prior_calc_progress, tmp_hash->prior_calc_progress, cli.idp[0]);
						}

						//Range sent a the end

					}
					
					
					if(tmp_hash->status != HASH_STATUS_FOUND)
					{
		    			n = socket_send_int(sockfd, CODE_OK);
						printf("Send range\n");

						queue_enqueue((void*)&(cli.range_cli), (void*)tmp_range);

						n = socket_send_int(sockfd, tmp_hash->type);
						n = socket_send_int(sockfd, tmp_range->id_hash);
						n = socket_send_mpz_t(sockfd, tmp_range->start);
						n = socket_send_mpz_t(sockfd, tmp_range->nb_op);
					}
					else
					{
		    			n = socket_send_int(sockfd, CODE_NO_HASH);
					}
            	 }

            	 break;

            case CODE_FOUND:
				n = socket_receive_int(sockfd, &tmp_return);
				
				for(i = 0; i < list_get_size((void*)p->hash_list); i++)
				{
					tmp_hash = (hash_t*)list_get((void*)p->hash_list, i);	
					if(tmp_hash->id == tmp_return)
					{
						break;
					}
				}

				tmp_hash->status = HASH_STATUS_FOUND;
				tmp_hash->end_time = time(NULL);

				n = socket_receive_string(sockfd, tmp_hash->clear_password);

				printf("Password found hash %d : %s\n", tmp_hash->id, tmp_hash->clear_password);


            	break;

            case CODE_QUIT:
	
				/** When it's ranges are done, remove client **/


            	break;

            case CODE_RANGE_DONE:

                tmp_range = (range_t*)queue_dequeue(&(cli.range_cli));

				/** Receive computing time **/

				n = socket_receive_int(sockfd, &tmp_return);


				for(i = 0; i < list_get_size((void*)p->hash_list); i++)
				{
					tmp_hash = (hash_t*)list_get((void*)p->hash_list, i);
					if(tmp_range->id_hash == tmp_hash->id)
					{
						break;
					}
				}


				if(tmp_hash != NULL && tmp_hash->id == tmp_range->id_hash)
				{
					if(tmp_return < TARGET_TIME)
					{
						cli.idp[tmp_hash->type] += cli.idp[tmp_hash->type] / 100;  
					}
					else if(tmp_return > TARGET_TIME)
					{
						cli.idp[tmp_hash->type] -= cli.idp[tmp_hash->type] / 100;  
					}
				}

				/** CODE_QUIT continue here **/

            	break;
        }

		if(n<=0)
		{
			client_quit(p->client_list, &cli, p->range_todo);
			return;
		}

    }

}

void client_quit(list* clients, client* cli, queue* range_todo)
{
    /** Put client range in range todo **/

	printf("Client quit!!!\n");
    while(!queue_isempty((void*)&(cli->range_cli)))
    {
		queue_enqueue((void*)range_todo, queue_dequeue((void*)&(cli->range_cli)));
		
    }
    /** Delete client **/

    client* tmp_cli;
    int i;

    for(i=0; i < list_get_size((void*)clients); i++)
    {
        tmp_cli = list_get((void*)clients, i);
        if(cli->socket_id  == tmp_cli->socket_id)
		{
            break;
		}
    }
    list_remove((void*)clients, i);

}



#include "api_executor.h"


int api_exec_list_hashes(list* hashes, json_t *resp_root)
{
	int i = 0;
	hash_t *tmp_hash;
	json_t *hashes_array;

	hashes_array = json_array();

	for(i=0; i<list_get_size(hashes); i++)
	{
        tmp_hash = (hash_t*)list_get((void*)hashes, i);
		json_array_append(hashes_array, json_integer(tmp_hash->id));
	}
	json_object_set_new(resp_root, "hashes", hashes_array);

	return 0;
}


int api_exec_list_clients(list* clients, json_t *resp_root)
{
	int i;
	client *tmp_client;
	json_t *clients_array;

	clients_array = json_array();

	for(i=0; i<list_get_size(clients); i++)
	{
		tmp_client = (client*)list_get((void*)clients, i);
		json_array_append(clients_array, json_integer(tmp_client->socket_id));
	}

	json_object_set_new(resp_root, "clients", clients_array);

	return 0;
}


int api_exec_client_details(list* clients, int client_id, json_t *resp_root)
{
	int i;
	json_t *atr_list;
	client *tmp_client = NULL;

	for(i=0; i<list_get_size(clients); i++)
	{
		tmp_client = (client*)list_get(clients, i);
		if(tmp_client->socket_id == client_id)
		{
			break;
		}
		tmp_client = NULL;
	}

	if(tmp_client == NULL)
	{
		return -2;
	}
	
	atr_list = json_object();

	json_object_set_new(atr_list, "id", json_integer(tmp_client->socket_id));
	json_object_set_new(atr_list, "uptime", json_integer(tmp_client->connect_time));
	json_object_set_new(atr_list, "md5_idp", json_integer(tmp_client->idp[HASH_TYPE_MD5]));
	json_object_set_new(atr_list, "ip", json_string(tmp_client->ip));
	json_object_set_new(atr_list, "port", json_integer(tmp_client->port));

	json_object_set_new(resp_root, "client", atr_list);

	return 0;
}

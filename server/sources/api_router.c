#include "api_router.h"



int api_router_url_handler(char* url, list *url_args, char* method, list *clients, list *hashes, char **resp_string, pthread_mutex_t *mutex)
{
	json_t *resp_root;
	char* tmp = NULL;
	int ret;

	resp_root = json_object();


	tmp = (char*)malloc(sizeof(char)*strlen(url)+1);
	strcpy(tmp, url);

	if(tmp[strlen(tmp)-1] == '/')
	{
		tmp[strlen(tmp)-1] = '\0';
	}

	if(strcmp(method, "GET") == 0 && strcmp(tmp, "/hashes") == 0)
	{
		ret = api_exec_list_hashes(hashes, resp_root);
	}
	else if(strcmp(method, "GET") == 0 && strcmp(tmp, "/clients") == 0)
	{
		ret = api_exec_list_clients(clients, resp_root);
	}
	else
	{
		ret = 1;
	}

	free(tmp);

	if(ret == 0)
	{
		tmp = json_dumps(resp_root, 0);
		*resp_string = (char*) malloc(sizeof(char)*strlen(tmp)+1);
		strcpy(*resp_string, tmp);
	}
	else if(1)
	{
		*resp_string = (char*)malloc(sizeof(char)*strlen("Unknow url")+1);
		strcpy(*resp_string, "Unknow url");
	}

	return ret;
}

void* api_router_get_fallback(char *url, char *method, list *url_args)
{
	return NULL;
}

int api_router_init()
{
	return 0;
}

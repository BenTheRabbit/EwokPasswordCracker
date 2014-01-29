#include "api_httpd.h"

int api_httpd_daemon(void* params)
{

	struct MHD_Daemon *httpd_daemon;

	httpd_daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, 8888, NULL, NULL, &api_httpd_answer_to_connection, params, MHD_OPTION_END);

	if(NULL == httpd_daemon) return 1;

	while(1)
	{
		sleep(1);
	}

	MHD_stop_daemon(httpd_daemon);

	return 0;
}

int api_httpd_get_url_args(void* cls, enum MHD_ValueKind kind, const char* key, const char* value)
{
	api_url_arg_pair_t *tmp_url_arg = NULL;
	char *tmp_string = NULL;

	tmp_url_arg = (api_url_arg_pair_t*)malloc(sizeof(api_url_arg_pair_t));

	tmp_string = (char*)malloc(sizeof(char)*strlen(key)+1);
	strcpy(tmp_string, key);
	tmp_url_arg->key = tmp_string;

	tmp_string = (char*)malloc(sizeof(char)*strlen(value)+1);
	strcpy(tmp_string, value);
	tmp_url_arg->value = tmp_string;

	list_add(cls, tmp_url_arg);

	return MHD_YES;
}

int api_httpd_answer_to_connection(void *cls, struct MHD_Connection *connection,
                          const char *url, const char *method,
						  const char *version, const char *upload_data,
						  size_t *upload_data_size, void **con_cls)
{

	struct MHD_Response *response;
	int ret;
	char* string_response = NULL;
	api_thread_parameters_t *thread_params;
	list url_args;

	list_init(&url_args);

	thread_params = (api_thread_parameters_t*)cls;


	MHD_get_connection_values(connection, MHD_GET_ARGUMENT_KIND, api_httpd_get_url_args, &url_args);

	api_router_url_handler(url, &url_args, method, thread_params->clients, thread_params->hashes, &string_response, thread_params->mutexes);


	response = MHD_create_response_from_buffer(strlen(string_response), (void*)string_response, MHD_RESPMEM_PERSISTENT);
	MHD_add_response_header(response, "Content-Type", "application/json");

	ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
	MHD_destroy_response(response);

	//free(string_response);

	return ret;
}

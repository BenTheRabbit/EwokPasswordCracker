#include "module_md5.h"


int module_md5_send_hash(int sockfd, hash_t* h)
{
	int n;
	hash_md5 * tmp;
	tmp = (hash_md5*)h->hash;

	n = socket_send_int(sockfd, HASH_TYPE_MD5);
	if(n<=0)
		return n;

	n = socket_send_int(sockfd, h->id);
	if(n<=0)
		return n;

	n = socket_send_string(sockfd, tmp->hash);
	if(n<=0)
		return n;

	n = socket_send_string(sockfd, tmp->charset);
	if(n<=0)
		return n;


	return n;
}

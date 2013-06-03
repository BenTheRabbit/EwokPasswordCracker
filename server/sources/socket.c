#include "socket.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void socket_init(config *conf)
{

    conf->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (conf->sockfd < 0)
        error("ERROR opening socket");

    memset((char *) &(conf->serv_addr), 0, sizeof(conf->serv_addr));

    conf->serv_addr.sin_family = AF_INET;
    conf->serv_addr.sin_addr.s_addr = INADDR_ANY;
    conf->serv_addr.sin_port = htons(conf->server_port);

    if (bind(conf->sockfd, (struct sockaddr *) &(conf->serv_addr), sizeof(conf->serv_addr)) < 0)
        error("ERROR on binding");

    listen(conf->sockfd,5);
}



int socket_send_mpz_t(int sockid, mpz_t nb)
{
	char buffer[BUFFER_SIZE], tmp_buffer[BUFFER_SIZE];
	int n;

	memset(buffer, 0, BUFFER_SIZE);
	memset(tmp_buffer, 0, BUFFER_SIZE);

	gmp_sprintf(tmp_buffer, "%Zd", nb);
	buffer[0] = strlen(tmp_buffer);

	n = send(sockid, buffer, 1, 0);
	if(n < 0)
	{
		perror("ERROR writing socket");
		return n;
	}

	memset(buffer, 0, BUFFER_SIZE);
	strcpy(buffer, tmp_buffer);

	n = send(sockid, buffer, strlen(tmp_buffer), 0);
	if(n < 0)
	{
		perror("ERROR writing socket");
		return n;
	}

	return n;
}

int socket_receive_mpz_t(int sockid, mpz_t *nb)
{
	
	char buffer[BUFFER_SIZE];
	int n;
	int nb_char;

	memset(buffer, 0, BUFFER_SIZE);
	n = recv(sockid,buffer,1,0);
	if (n < 0)
	{
		perror("ERROR reading from socket");
		return n;
	}
	nb_char = buffer[0];

	memset(buffer, 0, BUFFER_SIZE);
	n = recv(sockid,buffer,nb_char,0);
	if (n < 0)
	{
		perror("ERROR reading from socket");
		return n;
	}

	mpz_set_str(*nb, buffer, 10);

	return n;

}

int socket_send_int(int sockid, int nb) 
{
	char buffer[BUFFER_SIZE], tmp_buffer[BUFFER_SIZE];
	int n;

	memset(buffer, 0, BUFFER_SIZE);
	memset(tmp_buffer, 0, BUFFER_SIZE);

	sprintf(tmp_buffer, "%d", nb);
	buffer[0] = strlen(tmp_buffer);

	n = send(sockid, buffer, 1, 0);
	if(n < 0)
	{
		perror("ERROR writing socket");
		return n;
	}

	memset(buffer, 0, BUFFER_SIZE);
	strcpy(buffer, tmp_buffer);

	n = send(sockid, buffer, strlen(tmp_buffer), 0);
	if(n < 0)
	{
		perror("ERROR writing socket");
		return n;
	}

	return n;
}

int socket_receive_int(int sockid, int *nb)
{
	char buffer[BUFFER_SIZE];
	int n;
	char nb_char;
	
	memset(buffer, 0, BUFFER_SIZE);
    n = recv(sockid,buffer,1,0);
    if (n < 0)
	{
        perror("ERROR reading from socket");
		return n;
	}
    
	nb_char = buffer[0];

	 memset(buffer, 0, BUFFER_SIZE);
    n = recv(sockid,buffer,nb_char,0);
    if (n < 0)
	{
        perror("ERROR reading from socket");
		return n;
	}

    *nb = atoi(buffer);


	
	return n;

}

int socket_send_string(int sockid, char *str)
{
	char buffer[BUFFER_SIZE], buffer_tmp[BUFFER_SIZE];
	int n;
	
	strcpy(buffer_tmp, str);
	memset(buffer, 0, BUFFER_SIZE);
	buffer[0] = strlen(buffer_tmp);
    n = send(sockid,buffer,1,0);
    if (n < 0)
	{
        perror("ERROR writing socket");
		return n;
  	}

    memset(buffer, 0, BUFFER_SIZE);
	strcpy(buffer, buffer_tmp);
    n = send(sockid,buffer,strlen(buffer_tmp),0);
    if (n < 0)
	{
        perror("ERROR writing socket");
		return n;
	}


	return n;

}

int socket_receive_string(int sockid, char *str)
{
	char buffer[BUFFER_SIZE];
	int n;
	char nb_char;
	
	memset(buffer, 0, BUFFER_SIZE);
    n = recv(sockid,buffer,1,0);
    if (n < 0)
	{
        perror("ERROR reading from socket");
		return n;
	}
    nb_char = buffer[0];

    memset(buffer, 0, BUFFER_SIZE);
    n = recv(sockid,buffer,nb_char,0);
    if (n < 0)
	{
        perror("ERROR reading from socket");
		return n;
	}

    strcpy(str, buffer);

	return n;

}


int socket_send_long(int sockid, long nb)
{
	char buffer[BUFFER_SIZE], tmp_buffer[BUFFER_SIZE];
	int n;

	memset(buffer, 0, BUFFER_SIZE);
	memset(tmp_buffer, 0, BUFFER_SIZE);

	sprintf(tmp_buffer, "%ld", nb);
	buffer[0] = strlen(tmp_buffer);

	n = send(sockid, buffer, 1, 0);
	if(n < 0)
	{
		perror("ERROR writing socket");
		return n;
	}

	memset(buffer, 0, BUFFER_SIZE);
	strcpy(buffer, tmp_buffer);

	n = send(sockid, buffer, strlen(tmp_buffer), 0);
	if(n < 0)
	{
		perror("ERROR writing socket");
		return n;
	}

	return n;
}

int socket_receive_long(int sockid, long *nb)
{
	char buffer[BUFFER_SIZE];
	int n;
	char nb_char;
	
	memset(buffer, 0, BUFFER_SIZE);
    n = recv(sockid,buffer,1,0);
    if (n < 0)
	{
        perror("ERROR reading from socket");
		return n;
	}

    nb_char = buffer[0];

	 memset(buffer, 0, BUFFER_SIZE);
    n = recv(sockid,buffer,nb_char,0);
    if (n < 0)
	{
        perror("ERROR reading from socket");
		return n;
    }

	*nb = strtol(buffer, NULL, 10);

	return n;
}



void socket_close(int sockfd)
{
	#if defined(unix) || defined(__APPLE__)
		close(sockfd);
	#elif defined(WIN32) || defined(WIN64)
		closesocket(sockfd);
	#endif
}

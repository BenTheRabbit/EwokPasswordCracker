#include "socket.h"


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int socket_connect(char *host, int port)
{
	int sockid = -1;
	do
	{
		printf("\nTry to reach %s:%d ...\n", host, port);
		sockid =  socket_try_connect(host, port);

		if(sockid < 0)
		{
			switch(ui_multi_choice_question("\n[R]etry, [c]hange parameters or [a]bort ?", "rca", 'r'))
			{
				case 'c':
					ui_input_string("Server address", host, host);
					port = ui_input_int("Server port", port, 0, 65535);

					config_set_string("client.conf", "SERVER_ADDRESS", host);
					config_set_int("client.conf", "SERVER_PORT", port);
					break;

				case 'a':
					printf("\nExiting ...\n\n");
					exit(0);
					break;
				
				case 'r':
				default:
					break;


			}
			printf("\n");
		}

	}while(sockid < 0);

	return sockid;
}

int socket_try_connect(char *host, int port)
{
	int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
	{
        perror("ERROR opening socket");
		return -1;
	}

    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        return -1;
    }
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);

    serv_addr.sin_port = htons(port);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        perror("ERROR connecting");
		return -1;
    }

	return sockfd;


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


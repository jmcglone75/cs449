#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <pthread.h>


void not_found(void *ptr);
void *accept_request(void *ptr);


void not_found(void *ptr)
{
	return;
}

void *accept_request(void *ptr)
{
	int *connfd = (int *)ptr;
	char curr = '\0';  //current char being read in - initialized to null
	int clrf_count = 0; //counter for number of CLRFs
	int str_count = 0; //position in string -- used to determine if message is > 4KB
	char buffer[4096];
	int i;

	while(clrf_count < 2 && str_count < 4096)
	{
		if(str_count > sizeof(buffer) + 1)
		{
			not_found(connfd);
			break;
		}

		int i = recv(*connfd, &curr, 1, 0); //reads one byte from recv
		if(i > 0)
		{
			if(curr == '\r')
			{
				i = recv(*connfd, &curr, 1, MSG_PEEK);
				if(i > 0 && curr == '\n')
					clrf_count++;
				else
					curr = '\n';
			}
			
			buffer[str_count] = curr;
			str_count++;
		}
	}
	buffer[str_count] = '\0';
	printf("buffer:\n");
	printf("%s\n", buffer);
	
	
	//get client ip and port # to write to stats.txt
	return NULL;
}


int main()
{
	int sfd, connfd;
	struct sockaddr_in addr;
	sfd = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(50770);
	addr.sin_addr.s_addr = INADDR_ANY;

	bind(sfd, (struct sockaddr *)&addr, sizeof(addr));

	int len = sizeof(addr);
	unsigned short port;
	char *ipstring;

/*	port = ntohs(addr.sin_port);
	ipstring = inet_ntoa(addr.sin_addr);

	printf("%s\n", ipstring);
	printf("port:%d\n", port);*/


	if(sfd == -1)
		printf("\nserver could not start\n\n");
	else
		printf("\nserver running...\n\n");

	pthread_t newthread;

	while(1)
	{
		listen(sfd, 10);	
		connfd = accept(sfd, (struct sockaddr *)&addr, &len);

		port = ntohs(addr.sin_port);
		ipstring = inet_ntoa(addr.sin_addr);

		printf("IP: %s\n", ipstring);
		printf("Port: %d\n", port);
		
		//need to make a struct to pass as argument to accept_request
		//can't just pass the connfd, also need to pass the client address in 
		//addition to the connfd.
		//need to use malloc to assign structs (not just one struct and keep overwriting		//it), because we are going to need that data thoughout the program

		
		if(connfd == -1)
			printf("client could not connect\n");
		else
			printf("client connected succesfully\n");
	
		//create thread that passes accept_request

		if(pthread_create(&newthread, NULL, (void *) accept_request, &connfd) != 0)
		{
			perror("pthread_create");
		}
		else
		{
			printf("thread successful\n\n");	
		}
	}
}

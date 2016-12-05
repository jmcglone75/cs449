#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <pthread.h>


void not_found(int client_socket);
void *accept_request(int client_socket);


void not_found(int client_socket)
{
	return;
}

void *accept_request(int client_socket)
{
	char curr = '\0';  //current char being read in - initialized to null
	int clrf_count = 0; //counter for number of CLRFs
	int str_count = 0; //position in string -- used to determine if message is > 4KB
	char buffer[4096];
	int i;

	while(clrf_count < 2 && str_count < 4096)
	{
		if(str_count > sizeof(buffer) + 1)
		{
			not_found(client_socket);
			break;
		}

		int i = recv(client_socket, &curr, 1, 0); //reads one byte from recv
		if(i > 0)
		{
			if(curr == '\r')
			{
				i = recv(client_socket, &curr, 1, MSG_PEEK);
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
	int server_socket, client_socket;
	struct sockaddr_in addr, client_name;
	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	int client_length = sizeof(client_name);
	
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(50770);
	addr.sin_addr.s_addr = INADDR_ANY;

	bind(server_socket, (struct sockaddr *)&addr, sizeof(addr));
	listen(server_socket, 10);


	if(server_socket == -1)
		printf("\nserver could not start\n\n");
	else
		printf("\nserver running...\n\n");

	pthread_t newthread;

	while(1)
	{
		client_socket = accept(server_socket, (struct sockaddr *)&client_name, &client_length);
		if(client_socket == -1)
			printf("client could not connect\n");
		else
			printf("client connected succesfully\n");
	
		//create thread that passes accept_request

		if(pthread_create(&newthread, NULL, (void *) accept_request, client_socket) != 0)
		{
			perror("pthread_create");
		}
		else
		{
			printf("thread successful\n\n");	
		}
	}
}

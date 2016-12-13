#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct Connection{
	int conn_fd;
	char *ip;
	int port;
};

void *accept_request(void *ptr)
{
	struct Connection *conn = (struct Connection*) ptr;
	int connfd = conn->conn_fd;
	char *ipString = conn->ip;
	int file_size;
	char *server_message;
	char curr = '\0';  //current char being read in - initialized to null
	int clrf_count = 0; //counter for number of CLRFs
	int str_count = 0; //position in string -- used to determine if message is > 4KB
	char buffer[4096];
	int i;

	while(clrf_count < 2 && str_count < 4096)
	{
		if(str_count > sizeof(buffer) + 1)
		{
			break;
		}

		int i = recv(connfd, &curr, 1, 0); //reads one byte from recv
		if(i > 0)
		{
			if(curr == '\r')
			{
				i = recv(connfd, &curr, 1, MSG_PEEK);
				if(i > 0 && curr == '\n')
					clrf_count++;
				
			}
			buffer[str_count] = curr;
			str_count++;
		}
	}
	buffer[str_count] = '\0';
	printf("buffer:\n");
	printf("%s\n", buffer);

	//check for http request

	if(strncmp(buffer, "GET ", 4) == 0)
	{
		//get filename
		char filename[50];
		int index = 5;
		while(buffer[index] != ' ')
		{
			filename[index-5] = buffer[index];
			index++;
		}
		printf("%s\n", filename);
		FILE *requested_file = fopen(filename, "r");
		if(requested_file != NULL) //file found.  send 200 OK
		{
			printf("file found\n");

			//get size of file
			fseek(requested_file, 0, SEEK_END);
			file_size = ftell(requested_file);
			fseek(requested_file, 0, SEEK_SET);

			server_message = (char *)malloc(250*sizeof(char) + file_size*sizeof(char));

			strcpy(server_message, "\nHTTP/1.1 200 OK\n");

			//get time
			char time_string[50];
			time_t current_time = time(NULL);
			struct tm *local_time = localtime(&current_time);
			strftime(time_string, 50, "Date: %A, %d %B %Y %X %Z\n", local_time);
			strcat(server_message, time_string);

			//append content length
			strcat(server_message, "Content-Length: ");
			char num[10];
			sprintf(num, "%d", file_size);
			strcat(server_message, num);

			strcat(server_message, "\nConnection: close\nContent-Type: text/html\n\n");

			char *content = (char *)malloc((file_size+1)*sizeof(char));
			fread(content, 1, file_size, requested_file);
			strcat(server_message, content);
		}
		else //file not found
		{
			server_message = (char *)malloc(50*sizeof(char));
			strcpy(server_message, "\nHTTP/1.1 404 Not Found\n");
			printf("file not found\n");
		}
	}
	else //http request not correct - default to 404
	{
		server_message = (char *)malloc(50*sizeof(char));
		strcpy(server_message, "\nHTTP/1.1 404 Not Found\n");
	}

	send(connfd, server_message, strlen(server_message), 0); //send server http response
	
	pthread_mutex_lock(&mutex);
	char *append_string = (char *)malloc(200*sizeof(char));
	char *client = (char *)malloc(40*sizeof(char));
	strcpy(append_string, buffer);
	strcpy(client, "Client:  ");
	strcat(client, ipString);
	strcat(client, ":50770\n");
	strcat(append_string, client);
	strcat(append_string, "\n--------------------------\n\n");
	FILE *output = fopen("stats.txt", "a+");
	fwrite(append_string, 1, strlen(append_string), output);
	fclose(output);
	pthread_mutex_unlock(&mutex);
	
	
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

	if(sfd == -1)
		printf("\nserver could not start\n\n");
	else
		printf("\nserver running...\n\n");

	pthread_t newthread;

	while(1)
	{
		listen(sfd, 10);	
		connfd = accept(sfd, (struct sockaddr *)&addr, &len);

		struct Connection *conn = malloc(sizeof(struct Connection));
		conn->conn_fd = connfd;
		conn->ip = inet_ntoa(addr.sin_addr);;
		conn->port = ntohs(addr.sin_port);	
		
		if(connfd == -1)
			printf("client could not connect\n");
		else
			printf("client connected succesfully\n");
	
		//create thread that passes accept_request
		int threadNum = pthread_create(&newthread, NULL, (void *) accept_request, conn) != 0;
		if(threadNum != 0)
		{
			perror("pthread_create");
		}
		else
		{
			pthread_join(newthread, NULL);
			printf("thread successful\n\n");	
		}
	}
}

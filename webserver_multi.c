#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "webserver.h"
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#define MAX_REQUEST 100

int n = 0;
pthread_mutex_t lock;
sem_t sem_empty;
sem_t sem_full;
int port, numThread;
int s;
int buffer[MAX_REQUEST + 1];

void *listener()
{
	int r;
    int sock;
	struct sockaddr_in sin;
	struct sockaddr_in peer;
	int peer_len = sizeof(peer);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);
	r = bind(sock, (struct sockaddr *) &sin, sizeof(sin));
	if(r < 0) {
		perror("Error binding socket:");
		return NULL;
	}
	r = listen(sock, 5);
	if(r < 0) {
		perror("Error listening socket:");
		return NULL;
	}
	printf("HTTP server listening on port %d\n", port);
    
	while (1)
	{
        int s;
            s = accept(sock, NULL, NULL);
            if (s < 0) break;
    		sem_wait(&sem_empty);
    		pthread_mutex_lock(&lock);
				buffer[n]=s;
				n++;  
    		pthread_mutex_unlock(&lock);
    		sem_post(&sem_full);
	}
	close(sock);
}

void *consumer()
{
	int i=0;
    for(i=0;i<sizeof(buffer);i++)
    {

		sem_wait(&sem_full);
		pthread_mutex_lock(&lock);

			int request_in;
			
			request_in = buffer[0];
			n --;
			int j=0;
			for(j=0;j<sizeof(buffer);j++)
			{
				buffer[j]=buffer[j+1];
			}
	
		pthread_mutex_unlock(&lock);
		sem_post(&sem_empty);
		process(request_in);
    }
}

void thread_control()
{
	int i=0;
	pthread_t myThreads[numThread+1];

	for(i=0; i<numThread+1; i++)
	{	
		if(i==0)
		{		
			pthread_create(&myThreads[i], NULL, listener, NULL);	
		}
		else
		{
			pthread_create(&myThreads[i], NULL, consumer, NULL);
		}
	
	}
	for(i=0; i<numThread+1; i++)
	{
		pthread_join(myThreads[i],NULL);
	}
}

int main(int argc, char *argv[])
{
	if(argc != 3 || atoi(argv[1]) < 2000 || atoi(argv[1]) > 50000)
	{
		fprintf(stderr, "./webserver_multi PORT(2001 ~ 49999) #_of_threads\n");
		return 0;
	}
	port = atoi(argv[1]);
	numThread = atoi(argv[2]);
	int i;
	pthread_t myThreads[numThread+1];
	pthread_mutex_init(&lock, NULL);
    sem_init(&sem_empty, 0, MAX_REQUEST);
    sem_init(&sem_full, 0, 0);
	thread_control();	
	return 0;
}


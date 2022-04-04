#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
//#include <linux/unistd.h>
#include<unistd.h>
#include <arpa/inet.h>
#include "webserver.h"
#include <string.h>
#include <stdlib.h>

int listener(int port)
{
		int sock;
		struct sockaddr_in sin;
		struct sockaddr_in peer;
		int peer_len = sizeof(peer);
		int r;

		sock = socket(AF_INET, SOCK_STREAM, 0);
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = INADDR_ANY;
		sin.sin_port = htons(port);
		r = bind(sock, (struct sockaddr *) &sin, sizeof(sin));
		if(r < 0) {
				perror("Error binding socket:");
				return -1;
		}

		r = listen(sock, 5);
		if(r < 0) {
				perror("Error listening socket:");
				return -1;
		}

		printf("HTTP server listening on port %d\n", port);
		while (1) {
				int fd;
				fd = accept(sock, NULL, NULL);
				if (fd < 0) {
						printf("Accept failed.\n");
						break;
				}
				process(fd);
		}
		close(sock);
 return 0;
}

int main(int argc, char *argv[]) {
		if(argc != 2 || atoi(argv[1]) < 2000 || atoi(argv[1]) > 50000)
		{
				fprintf(stderr, "./webserver PORT(2001 ~ 49999)\n");
				return 0;
		}

		int port = atoi(argv[1]);
		listener(port);

		return 0;
}

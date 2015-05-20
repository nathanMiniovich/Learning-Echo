#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

#define max(a,b) (a>b?a:b)
#define MAXBUFSIZE 1024

int main(int argc, char** argv){

	int socketDescriptor;
	int sockDes2;
	int msglen;
	char buffer[MAXBUFSIZE];
	struct sockaddr_in serverAdder;
	struct sockaddr_in clientAdder;
	socklen_t ic;
	int sockFlags;

	socketDescriptor = socket(AF_INET,SOCK_STREAM,0);
	memset(&serverAdder,0,sizeof(serverAdder));

	serverAdder.sin_family = AF_INET;
	serverAdder.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAdder.sin_port = htons(50000);

	bind(socketDescriptor, (struct sockaddr*) &serverAdder, sizeof(serverAdder));

	if((sockFlags = fcntl(socketDescriptor,F_GETFL, 0)) < 0){
		fprintf(stderr, "fcntl with F_GETFL failed, exiting\n");
		exit(0);
	}

	if(fcntl(socketDescriptor, F_SETFL, sockFlags | O_NONBLOCK) < 0){
		fprintf(stderr, "fcntl with F_SETFL failed, exiting\n");
		exit(0);
	}

	int sent;
	int sentTotal;
	int clientAdderLength;
	listen(socketDescriptor,1);
	fd_set readfds,writefds;
	int fdlist[1024];
	int max_fd = 0;
	int nfds;
	int i;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	for(; ;){

		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_CLR(0,&readfds);
		FD_CLR(0,&writefds);
		FD_SET(socketDescriptor,&readfds);
		FD_SET(socketDescriptor,&writefds);
		nfds = socketDescriptor;

		for (i=0; i<max_fd; i++) {

			nfds = max(nfds,fdlist[i]);

			if(fdlist[i]){
				FD_SET(fdlist[i],&readfds);
				FD_SET(fdlist[i], &writefds);
			}
		}

		if(select(nfds+1,&readfds,&writefds,NULL, &tv) != -1){

			if (FD_ISSET(socketDescriptor, &readfds)) {
				clientAdderLength = sizeof(clientAdder);
				fdlist[max_fd] = accept(socketDescriptor, (struct sockaddr*) &clientAdder, &clientAdderLength);
				max_fd++;

			}
			for (i=0; i<max_fd; i++) {
				if (FD_ISSET(fdlist[i], &readfds)) {
					sentTotal = 0;

					msglen = recv(fdlist[i],buffer,sizeof(buffer),0);

					if(msglen == 0){
						printf("Client socket closed\n");
						close(fdlist[i]);
						fdlist[i] = 0;
					}

					if(msglen == -1){
						continue;
					}

				}	

				if (FD_ISSET(fdlist[i], &writefds)) {
					while(sentTotal < msglen){
						sent = send(fdlist[i],&buffer[sentTotal],msglen - sentTotal, 0);
						sentTotal += sent;
					}
				}
			}
		}
	}

	exit(0);
}

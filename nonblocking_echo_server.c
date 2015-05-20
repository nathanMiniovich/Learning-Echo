#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

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
	listen(socketDescriptor,1);
	int clientAdderLength = sizeof(clientAdder);
	sockDes2 = accept(socketDescriptor, (struct sockaddr*) &clientAdder, &clientAdderLength);

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

	for(; ;){
		sentTotal = 0;

		msglen = recv(sockDes2,buffer,sizeof(buffer),0);

		if(msglen == 0){
			printf("No messages are available, peer has performed orderly shutdown, exiting\n");
			goto error;
		}

		if(msglen == -1){
			continue;
		}

		while(sentTotal < msglen){
			sent = send(sockDes2,&buffer[sentTotal],msglen - sentTotal, 0);
			sentTotal += sent;
		}
	}

error:
	close(sockDes2);
	exit(0);

}
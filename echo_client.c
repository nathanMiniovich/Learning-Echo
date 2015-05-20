#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define MAXBUFSIZE 1024

int main(int argc, char** argv){

	int socketDescriptor;
	char msg[MAXBUFSIZE];
	char echo[MAXBUFSIZE];

	struct sockaddr_in addr;

	memset(&addr,0,sizeof(addr));
	socketDescriptor = socket(AF_INET,SOCK_STREAM,0);

	addr.sin_family = AF_INET;
	inet_pton(AF_INET,"192.168.100.155",&addr.sin_addr.s_addr);
	addr.sin_port = htons(50000);

	connect(socketDescriptor,(struct sockaddr*)&addr, sizeof(addr));

	for(; ;){
		memset(msg,0,MAXBUFSIZE); //clear message and echo buffers on each iteration
		memset(echo,0,MAXBUFSIZE);

		fgets(msg,MAXBUFSIZE,stdin);
		send(socketDescriptor,msg,strlen(msg),0);
		recv(socketDescriptor,echo,sizeof(echo),0);

		printf("%s",echo);
		fflush(stdout); //for compatibility with older systems that need to have the stdout buffer dumped

	}

	exit(0);

}
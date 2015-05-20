#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define MAXBUFSIZE 1024

int main(int argc, char** argv){

	int socketDescriptor
	char msg[MAXBUFSIZE];
	char echo[MAXBUFSIZE];

	struct sockaddr_in addr;

	memset(&addr,0,sizeof(addr));
	socketDescriptor = socket(AF_INET,SOCK_STREAM,0);

}
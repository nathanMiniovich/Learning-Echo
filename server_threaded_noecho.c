#include	<sys/time.h>
#include	<sys/types.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<errno.h>
#include	<string.h>
#include	<sys/socket.h>
#include	<netdb.h>
#include	<pthread.h>
#include 	<math.h>
#include	<signal.h>

int claim_port( const char * port ){

	struct addrinfo	addrinfo;
	struct addrinfo* result;
	int	sd;
	char message[256];
	int	on = 1;

	addrinfo.ai_flags = AI_PASSIVE;		
	addrinfo.ai_family = AF_INET;		
	addrinfo.ai_socktype = SOCK_STREAM;	
	addrinfo.ai_protocol = 0;	
	addrinfo.ai_addrlen = 0;
	addrinfo.ai_addr = NULL;
	addrinfo.ai_canonname = NULL;
	addrinfo.ai_next = NULL;
	

	if ( getaddrinfo( 0, port, &addrinfo, &result ) != 0 ){

		fprintf( stderr, "\x1b[1;31mgetaddrinfo( %s ) failed errno is %s.  File %s line %d.\x1b[0m\n", port, strerror( errno ), __FILE__, __LINE__ );
		return -1;

	}

	else if ( errno = 0, (sd = socket( result->ai_family, result->ai_socktype, result->ai_protocol )) == -1 ){

		write( 1, message, sprintf( message, "socket() failed.  File %s line %d.\n", __FILE__, __LINE__ ) );
		freeaddrinfo( result );
		return -1;

	}

	else if ( setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) ) == -1 ){

		write( 1, message, sprintf( message, "setsockopt() failed.  File %s line %d.\n", __FILE__, __LINE__ ) );
		freeaddrinfo( result );
		close( sd );
		return -1;

	}

	else if ( bind( sd, result->ai_addr, result->ai_addrlen ) == -1 ){

		freeaddrinfo( result );
		close( sd );
		write( 1, message, sprintf( message, "\x1b[2;33mBinding to port %s ...\x1b[0m\n", port ) );
		return -1;

	}

	else{

		write( 1, message, sprintf( message,  "\x1b[1;32mSUCCESS : Bind to port %s\x1b[0m\n", port ) );
		freeaddrinfo( result );		
		return sd;		

	}

}

//under construction
void* printBot(void* arg){

	int sd;

	sd = *(int*)arg;
	free(arg);



	return;


}


void sig_handler(int signum){
	
	printf("Passed interrupt signal, server exiting\n");
	exit(0);

}

int main(int argc, char** argv){

	int socketDescriptor:
	char message[256];
	pthread_t threadId
	pthread_attr_t kernel_attr
	socklen_t ic:
	int fd;
	struct sockaddr_in senderAddr
	int* fdptr;

	struct sigaction action
	action.sa_flags = SA_SIGINFO | SA_RESTART;
	action.sa_sigaction = sig_handler;
	sigemptyset(&action.sa_mask);
	sigaction(SIGINT, &action, 0);

	if ( pthread_attr_init( &kernel_attr ) != 0 ){

		printf( "pthread_attr_init() failed in file %s line %d\n", __FILE__, __LINE__ );
		return 0;
	}

	else if ( pthread_attr_setscope( &kernel_attr, PTHREAD_SCOPE_SYSTEM ) != 0 ){
		
		printf( "pthread_attr_setscope() failed in file %s line %d\n", __FILE__, __LINE__ );
		return 0;
	}

	//port already in use
	else if ( (socketDescriptor = claim_port( "41465" )) == -1 ){
		
		write( 1, message, sprintf( message,  "\x1b[1;31mCould not bind to port %s errno %s\x1b[0m\n", "41465", strerror( errno ) ) );
		return 1;
	}

	else if ( listen( socketDescriptor, 100 ) == -1 ){
			
		printf( "listen() failed in file %s line %d\n", __FILE__, __LINE__ );
		close( socketDescriptor );
		return 0;
	}

	else{
		
		ic = sizeof(senderAddr);

		while ( (fd = accept( socketDescriptor, (struct sockaddr *)&senderAddr, &ic )) != -1 ){

			fdptr = (int *)malloc( sizeof(int) );
			*fdptr = fd;					// pointers are not the same size as ints any more.

			if ( pthread_create( &threadId, &kernel_attr, printBot, fdptr ) != 0 ){
				printf( "pthread_create() failed in file %s line %d\n", __FILE__, __LINE__ );
				return 0;
			}

			else{
				continue;
			}
		}
		close(socketDescriptor);
		free(fdptr);
		return 0;

}
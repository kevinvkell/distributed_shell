//Kevin Kell
// 3/21/13
#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/time.h>
#include <crypt.h>

int main(int argc, char* argv[]){

	int status;
	struct addrinfo *results;
	struct addrinfo hints;
	char message[512];
	char buffer[10000];
	char salt[512];
	int socket_descriptor;
	int bytes;
	int i;

	if(argc < 6) {
		fprintf(stderr, "usage: [address] [port] [username] [password] [command...]\n");
		exit(1);
	}

	char address[strlen(argv[1]) + 1];
	strcpy(address, argv[1]);

	char port[strlen(argv[2]) + 1];
	strcpy(port, argv[2]);

	char username[strlen(argv[3]) + 1];
	strcpy(username, argv[3]);

	char password[strlen(argv[4]) + 1];
	strcpy(password, argv[4]);

	char command[strlen(argv[5]) + 1];
	strcpy(command, argv[5]);


//clear the hints struct
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

//fill in the results struct using the hints
	if((status = getaddrinfo(address, port, &hints, &results)) != 0){
		perror("getaddrinfo");
		exit(1);
	}

//get the socket descriptor using the information gained from getaddrinfo
	if((socket_descriptor = socket(results->ai_family, results->ai_socktype, results->ai_protocol)) == -1){
		perror("socket");
		exit(1);
	}

//connect to the server 
	if (connect(socket_descriptor, results->ai_addr, results->ai_addrlen) == -1) {
		close(socket_descriptor);
		perror("connect");
		exit(1);
	}

	fprintf(stderr, "connected to server\nsending cridentials...\n");
//send username
	memset(message, 0, sizeof(message));
	sprintf(message, "%s", username);

	if(send(socket_descriptor, message, (sizeof(message) + 1), 0) == -1){
		perror("send");
		exit(1);
	}

	memset(buffer, 0, sizeof(buffer));

	if((bytes = recv(socket_descriptor, buffer, sizeof(buffer), 0)) == -1){
		perror("recv");
		exit(1);
	}

	memset(salt, 0, sizeof(salt));
	strcpy(salt, buffer);
//use received salt to encrypt password
	if(send(socket_descriptor, crypt(password, salt), (strlen(crypt(password, salt)) + 1), 0) == -1) {
		perror("send");
		exit(1);
	}

	memset(buffer, 0, sizeof(buffer));

	if((bytes = recv(socket_descriptor, buffer, sizeof(buffer), 0)) == -1){
		perror("recv");
		exit(1);
	}

	fprintf(stderr, "%s\n", buffer);
//extract command
	memset(message, 0, sizeof(message));
	strcpy(message, argv[5]);
	strcat(message, " ");
	for(i=6; i<argc; i++) {
		strcat(message, argv[i]);
		strcat(message, " ");
	}

	printf("sending command %s\n\n", message);
//send command
	if(send(socket_descriptor, message, (sizeof(message) + 1), 0) == -1){
		perror("send");
		exit(1);
	}
//receive results
	memset(buffer, 0, sizeof(buffer));
	while((bytes = recv(socket_descriptor, buffer, sizeof(buffer), 0)) > 0){
		printf("%s", buffer);
		memset(buffer, 0, sizeof(buffer));
	}
	if(bytes < 0) {
		perror("recv");
		exit(1);

	}

	close(socket_descriptor);
	freeaddrinfo(results);
	
	return 0;	
}

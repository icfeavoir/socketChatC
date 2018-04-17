#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>


#define MAX_PENDING_CONNECTIONS (1)

void sendMsg(int socket, char *msg){
	write(socket, msg, strlen(msg));
	printf("server just sent : %s", msg);
}

void *readData(void *param){
	int *socket = (int *)param;
	char msg[255];
	for(;;){
		// reinitialize msg
		memset(msg, 0, 255);
		int n = read(*socket, msg, 255);
		
		if (n < 0){
			printf("Error on read\n");
		}else{
			printf("Client sent : %s\n", msg);

			// seneding confirm message to client
			char valid[] = "server received : ";
			strcat(valid, msg);
			strcat(valid, "\n");
			sendMsg(*socket, valid);
		}
	}
}

int main(void){
	int socket_ecoute, socketClient;;
	struct sockaddr_in mon_adresse;

	socket_ecoute = socket(AF_INET, SOCK_STREAM, 0);
	mon_adresse.sin_family = AF_INET;
	mon_adresse.sin_port = htons (8081);
	mon_adresse.sin_addr.s_addr = htonl (INADDR_ANY);

	bind(socket_ecoute, (struct sockaddr *)&mon_adresse, sizeof (mon_adresse));

	listen(socket_ecoute, MAX_PENDING_CONNECTIONS);
	printf("listenning...\n");

	pthread_t threadListen;
	while(1){
		socketClient = accept(socket_ecoute, NULL, 0);
		printf("The client is connected\n");

		// welcome msg
		char welcome[] = "Welcome!\n";
		sendMsg(socketClient, welcome);

		// thread reading socket
		if(pthread_create(&threadListen, NULL, readData, &socketClient)){
			printf("An error occured when startint readData thread\n");
		}

	}

	close(socket_ecoute);
	return 0;
}
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#define MAX 512

/*
    ARGUMENTS:
    argv[1] - port number
*/

int main(int argc, char** argv) {
    //create the server socket
	char messageFromClient[MAX];
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(1);
    }

    //define server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(argv[1]));
    server_address.sin_addr.s_addr = INADDR_ANY;

    //bind the socket to our specified IP and port
    if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address))) {
        perror("Error binding socket to specified IP and port");
        exit(1);
    }

    listen(server_socket, 5);

    //accept the connection
    int client_socket;
    while (true) {
		client_socket = accept(server_socket, NULL, NULL);
		if (client_socket == -1) {
			perror("error accepting client socket");
			exit(1);
		}
        if (client_socket == -1) {
            perror("Accept connection failed");
            exit(1);
        }

        if (read(client_socket, messageFromClient, MAX) == -1) {
            perror("Error while reading socket");
            exit(1);
        }
        printf("Otrzymano: %s\n", messageFromClient);
    }

    close(server_socket);
    return 0;
}

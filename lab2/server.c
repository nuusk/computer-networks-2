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

#define PORT_NUMBER 9002

int main(int argc, char** argv) {
    //what the server has to say
    char buff[MAX] = "elo elo 3 2 0";

    //create the server socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(1);
    }

    //define server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;

    //bind the socket to our specified IP and port
    if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address))) {
        perror("Error binding socket to specified IP and port");
        exit(1);
    }

    listen(server_socket, 5);

    //accept the connection
    int client_socket;
    while (client_socket = accept(server_socket, NULL, NULL) != -1) {
        if (client_socket == -1) {
            perror("Accept connection failed");
            exit(1);
        }

        if (write(client_socket, buff, MAX) == -1) {
            perror("Error writing to the client socket");
            exit(1);
        }
    }

    close(server_socket);
    return 0;
}

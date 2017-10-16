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
    //where we will store the message from the server
    char buff[MAX];

    //create a socket
    int network_socket;
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    //specify an address for the socket to connect to
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;        //now we know what type of address we work with
    server_address.sin_port = htons(9002);

    //now we modify the actual address
    server_address.sin_addr.s_addr = INADDR_ANY;

    //connect
    int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    if (connection_status == -1) {
        perror("Error conecting to the remote socket");
        exit(1);
    }

    read(network_socket, buff, MAX);
    printf("Otrzymano: %s", buff);

    close(network_socket);

    return 0;
}

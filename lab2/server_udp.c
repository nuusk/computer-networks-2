//simple UDP server
//use with one argument, which is port number

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024
#define PORT_NUMBER argv[1]

//make endless_loop 'false' if you want to echo a datagram only once
#define ENDLESS_LOOP 1

int main(int argc, char **argv) {
    int sockfd;               /* socket */
    int clientlen;            /* byte size of client's address */
    struct sockaddr_in serveraddr; /* server's addr */
    struct sockaddr_in clientaddr; /* client addr */
    char buf[BUFSIZE];        /* message buf */
    int n;                    /* message byte size */

    //input error control
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments\nTry: %s <port_number>\n", argv[0]);
        exit(1);
    }

    //create parent socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);        //SOCK_DGRAM is for UDP
    if (sockfd == -1) {
        perror("Error while opening socket");
        exit(1);
    }

    //build the server's Internet address
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(atoi(PORT_NUMBER));


    if (bind(sockfd, (struct sockaddr *) &serveraddr,
    sizeof(serveraddr)) < 0) {
        perror("Error while binding socket");
        exit(1);
    }

    //printf("Server started running on port %s...", PORT_NUMBER);

    //main loop: wait for a datagram, then echo it
    clientlen = sizeof(clientaddr);

    do {
    bzero(buf, sizeof(buf));
    //receive data from the client
    n = recvfrom(sockfd, buf, BUFSIZE, 0,
        (struct sockaddr *) &clientaddr, &clientlen);
    if (n < 0) {
        perror("ERROR while receiving data (recvfrom)");
        exit(1);
    }

    //what does the fox say
    printf("Client said: %s\n", buf);

    //send message to the client
    n = sendto(sockfd, "Got your message, bud.\n", 20, 0,
        (struct sockaddr *) &clientaddr, clientlen);

    if (n < 0) {
        perror("ERROR while sending message to the client");
        exit(1);
    }
} while (ENDLESS_LOOP);
}

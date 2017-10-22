//simple UDP client
//use with two arguments: <host> <port>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFSIZE 1024
#define PORT_NUMBER argv[2]
#define HOST argv[1]


int main(int argc, char **argv) {
    int sockfd, portno, n;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];

    //input error control
    if (argc != 3) {
       fprintf(stderr,"Wrong number of arguments\n Try: %s <hostname> <port>\n", argv[0]);
       exit(0);
    }
    hostname = HOST;
    portno = atoi(PORT_NUMBER);

    //create the socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    //gethostbyname: get the server's DNS entry
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    //build the server's Internet address
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);

    //get a message from the user
    bzero(buf, BUFSIZE);
    printf("Please enter msg: ");
    fgets(buf, BUFSIZE, stdin);

    //send the message to the server
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, buf, strlen(buf), 0, &serveraddr, serverlen);
    if (n < 0) {
        perror("ERROR in sendto");
        exit(1);
    }

    //print the server's reply
    n = recvfrom(sockfd, buf, strlen(buf), 0, &serveraddr, &serverlen);
    if (n < 0) {
        perror("ERROR in recvfrom");
        exit(1);
    }
    printf("Echo from server: %s", buf);
    return 0;
}

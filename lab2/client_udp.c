//simple UDP client
//use with two arguments: <host> <port> <msg>

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
#define MESSAGE argv[3]


int main(int argc, char **argv) {
    int sockfd, n;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *hp;
    char buf[BUFSIZE];

    //input error control
    if (argc != 4) {
       fprintf(stderr,"Wrong number of arguments\n Try: %s <hostname> <port> <msg>\n", argv[0]);
       exit(0);
    }

    //create the socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);        //SOCK_DGRAM is dor UDP
    if (sockfd < 0) {
        perror("ERROR while creating socket");
        exit(1);
    }

    //build the server's Internet address
    serveraddr.sin_family = AF_INET;
    hp = gethostbyname(HOST);
    bcopy((char *)hp->h_addr, (char *)&serveraddr.sin_addr, hp->h_length);
    if (hp == 0) {
        perror("Unknown host");
        exit(1);
    }
    serveraddr.sin_port = htons(atoi(PORT_NUMBER));


    //send the message to the server
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, MESSAGE, strlen(MESSAGE), 0, &serveraddr, serverlen);
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

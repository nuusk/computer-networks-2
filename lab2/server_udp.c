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
    int portno;               /* port to listen on */
    int clientlen;            /* byte size of client's address */
    struct sockaddr_in serveraddr; /* server's addr */
    struct sockaddr_in clientaddr; /* client addr */
    struct hostent *hostp;    /* client host info */
    char buf[BUFSIZE];        /* message buf */
    char *hostaddrp;          /* dotted decimal host addr string */
    int optval;               /* flag value for setsockopt */
    int n;                    /* message byte size */

    //input error control
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments\nTry: %s <port_number>\n", argv[0]);
        exit(1);
    }
    portno = atoi(PORT_NUMBER);

    //create parent socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
      perror("Error while opening socket");
      exit(1);
    }

    /* setsockopt: Handy debugging trick that lets
    * us rerun the server immediately after we kill it;
    * otherwise we have to wait about 20 secs.
    * Eliminates "ERROR on binding: Address already in use" error.
    */

    optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
	     (const void *)&optval , sizeof(int));


         //build the server's Internet address
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)portno);


    if (bind(sockfd, (struct sockaddr *) &serveraddr,
    sizeof(serveraddr)) < 0) {
        perror("Error while binding socket");
        exit(1);
    }

    //main loop: wait for a datagram, then echo it
    clientlen = sizeof(clientaddr);

    do {
    //receive data from the client
    bzero(buf, BUFSIZE);
    n = recvfrom(sockfd, buf, BUFSIZE, 0,
        (struct sockaddr *) &clientaddr, &clientlen);
    if (n < 0) {
        perror("ERROR in recvfrom");
        exit(1);
    }

    /*
    //gethostbyaddr: determine who sent the datagram
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
			  sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    if (hostp == NULL) {
        perror("ERROR on gethostbyaddr");
        exit(1);
    }
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL) {
        perror("ERROR on inet_ntoa\n");
        exit(1);
    }
    printf("server received datagram from %s (%s)\n",
        hostp->h_name, hostaddrp);

    */
    
    printf("server received %d/%d bytes: %s\n", strlen(buf), n, buf);



    //sendto: echo the input back to the client
    n = sendto(sockfd, buf, strlen(buf), 0,
        (struct sockaddr *) &clientaddr, clientlen);

    if (n < 0) {
        perror("ERROR in sendto");
        exit(1);
    }
} while (ENDLESS_LOOP);
}

#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <mach/error.h>
#include <ctype.h>
const int one = 1;

int main(int argc, char ** argv){
    if(argc!=2)
        error(1,0,"Usage: %s <port>", argv[0]);
    
    sockaddr_in localAddress{
        .sin_family = AF_INET,
        .sin_port   = htons(atoi(argv[1])),
        .sin_addr   = {htonl(INADDR_ANY)}
    };
    
    int servSock = socket(PF_INET, SOCK_STREAM, 0);
    setsockopt(servSock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    
    if(bind(servSock, (sockaddr*) &localAddress, sizeof(localAddress)))
        error(1,errno,"Bind failed!");
    
    listen(servSock, 1);

    while(true) {
        int clientSock = accept(servSock, nullptr, nullptr);
        printf("Accepted a new connection\n");
        while(true){
            uint16_t msgSize;
            if(recv(clientSock, &msgSize, sizeof(uint16_t), MSG_WAITALL) != sizeof(uint16_t))
                break;
            msgSize = ntohs(msgSize);
            char data[msgSize+1]{};
            if(recv(clientSock, data, msgSize, MSG_WAITALL) != msgSize)
                break;
            printf(" Received %2d bytes: ⎹%s⎸\n", msgSize, data);
        }
        close(clientSock);
        printf("Connection closed\n");
    }
}

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define HOST_PORT 13
#define MAX 512
using namespace std;

//argv[1] is server addres
//argv[2] is port number

int main(int argc, char **argv) {
    sockaddr_in sock_addr;
    int lbajt;
    char buff[MAX];

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1) {
        perror("Error creating socket.");
        exit(1);
    }

    sock_addr.sin_family = AF_INET;
    inet_aton(argv[1], &sock_addr.sin_addr);
    sock_addr.sin_port = htons(argv[2]);

    int result = connect(sock, (sockaddr*)&sock_addr, sizeof(sock_addr));
    if (result == -1) {
        perror("Error conecting");
        exit(1);
    }

    while((lbajt = read(sock, buff, MAX)) > 0) {
        printf("result:  %s", buff);
    }

    return 0;
}

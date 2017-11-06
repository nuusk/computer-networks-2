#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <error.h>
#include <iostream>
#include <random>
#include <chrono>
#include <ctype.h>
std::default_random_engine gen(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

int main(int argc, char ** argv){
    if(argc!=3)
        error(1,0,"Usage: %s <ip> <port>", argv[0]);
    
    addrinfo hints {};
    hints.ai_family = AF_INET;
    hints.ai_protocol = IPPROTO_TCP;
    
    addrinfo *resolved;
    if(int err = getaddrinfo(argv[1], argv[2], &hints, &resolved))
        error(1, 0, "Resolving address failed: %s", gai_strerror(err));
    
    int sock = socket(resolved->ai_family, resolved->ai_socktype, resolved->ai_protocol);
    if(connect(sock, resolved->ai_addr, resolved->ai_addrlen))
        error(1,errno, "Failed to connect");
    freeaddrinfo(resolved);

    std::uniform_int_distribution<int> dist(11,19);
    const int msgCnt = 20;
    
    for(int i = 0; i < msgCnt; i++){
        std::string msg(std::to_string(i));
        msg.resize (dist(gen),'.');
        uint16_t msgSize = htons(msg.length());
        write(sock, &msgSize, sizeof(uint16_t));
        write(sock, msg.c_str(), msg.length());
    }
    close(sock);
    return 0;
}

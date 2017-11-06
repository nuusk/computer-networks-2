#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <iostream>
/*
~~~~~ tcp client ~~~~~
~ connects to the given address and sends data in a loop
~ usage: <ip> <port> ~
~ poe
~~~~~~~~~~~~~~~~~~~~~~
*/

#define DATA_SENT 10

int main(int argc, char **argv) {
  if (argc != 3) {
    perror("wrong arguments.");
    printf("Try: %s <ip> <port>\n", argv[0]);
    exit(1);
  }

  addrinfo hints {};
  hints.ai_family = AF_INET;
  hints.ai_protocol = IPPROTO_TCP;

  addrinfo *resolved;
  int err = getaddrinfo(argv[1], argv[2], &hints, &resolved);
  if (err) {
    perror("Error while resolving address");
    exit(1);
  }

  int server_socket = socket(resolved->ai_family, resolved->ai_socktype, resolved->ai_protocol);
  if(connect(server_socket, resolved->ai_addr, resolved->ai_addrlen)) {
    perror("Error while connecting to the server socket");
    exit(1);
  }
  fcntl(server_socket, F_SETFL, O_NONBLOCK, 1);

  freeaddrinfo(resolved);

  char data[DATA_SENT];
  int sent = 1;
  while(true) {
    int written = write(server_socket, data, sizeof(data));
    if (written==DATA_SENT) {
        std::cout << "   Sent " << sent++ << "0\r" << std::flush;
    } else if (written==-1) {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
            std::cout << std::endl << "Next write would block" << std::endl;
        else
            std::cout << std::endl << "Write went wrong" << std::endl;
        break;
    } else {
        std::cout << std::endl << "Sent only " << written << " out of 10 bytes" << std::endl;
        break;
    }
  }
  close(server_socket);

  return 0;
}

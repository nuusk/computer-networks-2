#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>

/*
~~~~~ tcp client ~~~~~
~ connects to the given address and sends data in a loop
~ usage: <ip> <port> ~
~ poe
~~~~~~~~~~~~~~~~~~~~~~
*/

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

  freeaddrinfo(resolved);

  char data[1];
  int sent = 1;
  while(true) {
    write(server_socket, data, sizeof(data));
    std::cout << "   Sent " << sent++ << "0\r" << std::flush;
  }

  return 0;
}

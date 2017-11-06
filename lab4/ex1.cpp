#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>

/*
~~~~~ tcp server ~~~~~
~ accepts new connections and does nothing about them
~ usage: <ip> <port> ~
~ poe
~~~~~~~~~~~~~~~~~~~~~~
*/

const int one = 1;

int main(int argc, char ** argv) {
  if (argc != 3) {
    perror("wrong arguments.");
    printf("Try: %s <ip> <port>\n", argv[0]);
    exit(1);
  }

  int network_socket = socket(PF_INET, SOCK_STREAM, 0);
  setsockopt(network_socket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

  sockaddr_in localAddress;
  localAddress.sin_family = AF_INET;
  localAddress.sin_port = htons(atoi(argv[2]));
  localAddress.sin_addr.s_addr = inet_addr(argv[1]);

  if (bind(network_socket, (sockaddr*) &localAddress, sizeof(localAddress))) {
    perror("Error while binding socket!");
    exit(1);
  }

  listen(network_socket, 1);

  while(true) {
      accept(network_socket, nullptr, nullptr);
      printf("Accepted a new connection, ignoring it.\n");
  }

  return 0;
}

#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <thread>
#include <atomic>
#include <netdb.h>
#include <mach/error.h>


//two arguments:
// argv[1] hostname,
// argv[2] port

using namespace std;

int main(int argc, char **argv) {
	// // znajdzie dowolne adresy, w tym IPv6, nie ustawi portu
	int client_socket;
	addrinfo * aio;
	int res = getaddrinfo(argv[1], argv[2], 0, &aio);

	// znajdzie tylko adresy IPv4, ustawi port 13
	//addrinfo * aio, aih {.ai_flags=0, .ai_family=AF_INET, .ai_socktype=SOCK_STREAM};
	//int res = getaddrinfo("fc.put.poznan.pl", "13", &aih, &aio);

	if(res) {
		perror("getaddrinfo failed");
		exit(1);
	}
	if(!aio) {
		perror("empty result");
		exit(1);
	}
	cout << "fc.put.poznan.pl" << endl;
	for(addrinfo * it = aio; ; it=it->ai_next){
		cout << "  " << inet_ntoa(((sockaddr_in*)it->ai_addr)->sin_addr) << endl;
		if(!it->ai_next) break;
	}

	client_socket = socket(aio->ai_family, SOCK_STREAM, 0);
	if (client_socket == -1) {
		perror("Error while creating socket");
		exit(1);
	}

	int result = connect(client_socket, aio->ai_addr, sizeof(sockaddr));

	freeaddrinfo(aio);
}

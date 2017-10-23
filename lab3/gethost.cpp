#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <thread>
#include <atomic>
#include <unistd.h>

using namespace std;

void print(hostent* ret){
	cout << ret->h_name << endl;
	for(auto it = ret->h_addr_list; *it; ++it){
			cout << "  " << inet_ntoa(*((in_addr*)*it)) << endl;
	}
}

int main(int argc, char **argv) {
	atomic<bool> wait1 {true}, wait2 {true};

	std::thread
	t1([&]{
		while(wait1.load());
		cout << "About to invoke gethostbyname(\"spam.org\")" << endl;
		hostent* ret2 = gethostbyname("spam.org");
		cout << "gethostbyname(\"spam.org\") finished" << endl << endl;
		wait2.store(false);
		sleep(1);
		printf("ret2: %p, ", ret2);
	}),
	t2([&]{
		cout << "About to invoke gethostbyname(\"fc.put.poznan.pl\")" << endl;
		hostent* ret = gethostbyname("fc.put.poznan.pl");
		cout << "gethostbyname(\"fc.put.poznan.pl\") finished" << endl << endl;
		wait1.store(false);
		while(wait2.load());
		cout << "Printing the result of gethostbyname(\"fc.put.poznan.pl\")" << endl;
		printf("ret: %p, ", ret);
	});

	t1.join();
	t2.join();
	return 0;
}

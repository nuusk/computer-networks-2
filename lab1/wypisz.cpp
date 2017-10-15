#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#define MAX 512
using namespace std;

int main(int argc, char **argv) {
    char buf[MAX];
    int lbajt;
    int plik;
    int cel;
    plik = open("plik.txt", O_RDONLY);
    if (plik == -1) {
        perror("Blad otwarcia pliku");
        exit(1);
    }
    cel = open("cel.txt", O_WRONLY);
    if (cel == -1) {
        perror("Blad otawrcia pliku do zapisu");
        exit(1);
    }

    while((lbajt = read(plik, buf, MAX)) > 0) {
        if (write(cel, buf, lbajt) == -1) {
            perror("Blad zapisu pliku");
            exit(1);
        }
        printf("otrzymano:  %s", buf);
    }    
    return 0;
}

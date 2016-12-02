#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "iftun.h"
#include "ext_in.h"
#include "ext_out.h"

// hote est soit VM3-6 ipv6 soit VM1-6 ipv6

int main(int argc, char *argv[]){
	char hote[128];
	char nomtun[128] = "tun0";
	int fd;
	
	fd = tun_alloc(nomtun);
	if(fd<0)return 1;
	
	printf("tun up == %d\n", fd);
	system("bash configure-tun.sh");
	
	if (fork() == 0){
		printf("mode serveur\n");
		ext_out();
	}
	else{
		printf("j'att l'appuis d'une touche pour lancer le client");
		getchar();
		printf("mode client\n");
		strcpy(hote, argv[2]);	
		ext_in(fd ,hote);
	}
	return EXIT_SUCCESS;
}


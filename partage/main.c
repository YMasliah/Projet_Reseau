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
	
	if (atoi(argv[1]) == 1){
		printf("mode serveur\n");
		ext_out();
	}
	else{
		printf("mode client\n");
		fd = tun_alloc(nomtun);
		if(fd<0)return 1;
		strcpy(hote, argv[2]);	
		ext_in(fd ,hote);
	}
	return EXIT_SUCCESS;
}


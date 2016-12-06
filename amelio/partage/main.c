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
// tunnel46d doit donner en argument a main :
// nomtunnel, ipv6_serveur, port_in, options_tunnel, port_out
// il doit aussi ecrire dans le script les routes et ipforward 

int main(int argc, char *argv[]){
	char ipServeur[100],nomTun[100],portIn[100],portServeur[100],optionsTun[100];
	int fd;
	
	if(argc < 6){ 
		printf("il faut 6 arguments : nomtunnel, options_tunnel, ipv6_serveur, port_serveur, port_out\n ");
		return 0;
	}
	
	strcpy(nomTun, argv[1]);
	strcpy(optionsTun, argv[2]);
	strcpy(ipServeur, argv[3]);
	strcpy(portServeur, argv[4]);
	strcpy(portIn, argv[5]);
	
	fd = tun_alloc(nomTun,optionsTun);
	if(fd<0)return 1;
	
	printf("tun up == %d\n", fd);
	system("bash configure-tun.sh");
	
	if (fork() == 0){
		printf("mode serveur\n");
		ext_out(fd,portIn);
	}
	else{
		printf("j'att l'appuis d'une touche pour lancer le client\n");
		getchar();
		printf("mode client\n");
		ext_in(fd ,ipServeur , portServeur);
	}
	return EXIT_SUCCESS;
}


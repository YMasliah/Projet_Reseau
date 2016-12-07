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
#include "conf_manager.h"

// hote est soit VM3-6 ipv6 soit VM1-6 ipv6
// tunnel46d doit donner en argument a main :
// nomtunnel, ipv6_serveur, port_in, options_tunnel, port_out
// il doit aussi ecrire dans le script les routes et ipforward 

int main(int argc, char *argv[]){
	char ipServeur[100], ipTun[100],
	     nomTun[100],portIn[100],portServeur[100],
	     optionsTun[100], net1[100], net2[100];
	int fd;

	char configFile[100];
	strcpy(configFile, argv[1]);
	if(argc != 2){ 
		printf("il faut 1 argument : fichierDeConfiguration\n ");
		return 0;
	}
	
	readConf(configFile, nomTun, ipTun, portIn, optionsTun, ipServeur,portServeur, net1, net2);

	writeScript("configure-tun.sh", nomTun, ipTun, net1, net2);

	fd = tun_alloc(nomTun,optionsTun);
	if(fd<0)return 1;
	
	printf("tun up == %d\n", fd);
	system("bash configure-tun.sh");
	
	if (fork() == 0){
		printf("mode serveur\n");
		ext_out(fd,portIn);
	}
	else{
		printf("j'attends l'appuis d'une touche pour lancer le client\n");
		sleep(5);
		printf("mode client\n");
		ext_in(fd ,ipServeur , portServeur);
	}
	return EXIT_SUCCESS;
}


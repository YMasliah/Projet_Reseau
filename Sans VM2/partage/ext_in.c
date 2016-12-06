/*  echo / client simple
    Master Informatique 2012 -- Université Aix-Marseille  
    Emmanuel Godard
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "ext_in.h"

#define MAXLIGNE 64

void ext_in(int fd, char* ipServeur, char* port)
{  
//  char * port; /* port TCP du serveur */   
  char ip[NI_MAXHOST]; /* adresse IPv4 en notation pointée */
  struct addrinfo *resol; /* struct pour la résolution de nom */
  int s; /* descripteur de socket */
  
//  port="123"; /* port TCP du serveur */   

  /* Résolution de l'hôte */
  if ( getaddrinfo(ipServeur,port,NULL, &resol) < 0 ){
    perror("résolution adresse");
    exit(2);
  }

  /* On extrait l'addresse IP */
  
  sprintf(ip,"%s",inet_ntop(AF_INET6, resol, ip, sizeof(struct sockaddr_in6)));

  /* Création de la socket, de type TCP / IP */
  /* On ne considère que la première adresse renvoyée par getaddrinfo */
  if ((s=socket(resol->ai_family,resol->ai_socktype, resol->ai_protocol))<0) {
    perror("allocation de socket");
    exit(3);
  }
  fprintf(stderr,"le n° de la socket est : %i\n",s);

  /* Connexion */
  fprintf(stderr,"Essai de connexion à %s (%s) sur le port %s\n\n",
	  ipServeur,ip,port);
  if (connect(s,resol->ai_addr,sizeof(struct sockaddr_in6))<0) {
    perror("connexion");
    exit(4);
  }
  freeaddrinfo(resol); /* /!\ Libération mémoire */

  /* Session */
  while( 1 ) { 
    /* Jusqu'à fermeture de la socket (ou de stdin)     */
    /* recopier à l'écran ce qui est lu dans la socket  */
    /* recopier dans la socket ce qui est lu dans stdin */
			recup_paquet(fd,s);
    }
  /* Destruction de la socket */
  close(s);

  fprintf(stderr,"Fin de la session.\n");
}

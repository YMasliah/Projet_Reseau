/* echo / serveur simpliste
   Master Informatique 2012 -- Université Aix-Marseille  
   Emmanuel Godard
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* taille maximale des lignes */
#define MAXLIGNE 80
#define CIAO "Au revoir ...\n"

void ext-outmessage(int f, char* hote, char* port);
void ext-in();
void ext-out();

int main(int argc, char *argv[]){

	ext-out();
	ext-in();
	return EXIT_SUCCESS;
}
#define MAXLIGNE 64

int ext-in()
{
  char * hote; /* nom d'hôte du  serveur */   
  char * port; /* port TCP du serveur */   
  char ip[NI_MAXHOST]; /* adresse IPv4 en notation pointée */
  struct addrinfo *resol; /* struct pour la résolution de nom */
  int s; /* descripteur de socket */
	int ipv4_6 = 0,i; /*0 = ipv4, 1 = ipv6*/

  hote="172.16.2.1"; /* nom d'hôte du  serveur */   
  port="123"; /* port TCP du serveur */   

	for(i=0;i<strlen(hote);i++)
		if(hote[i] == ':'){
			printf("ipv6\n");
			ipv4_6 = 1;
			break;
		}
  printf("%s",hote);
  
  /* Résolution de l'hôte */
  if ( getaddrinfo(hote,port,NULL, &resol) < 0 ){
    perror("résolution adresse");
    exit(2);
  }

  /* On extrait l'addresse IP */
 	if(ipv4_6 == 0)
		inet_ntop(AF_INET, resol, ip, NI_MAXHOST);
  else
		inet_ntop(AF_INET6, resol, ip, NI_MAXHOST);

  /* Création de la socket, de type TCP / IP */
  /* On ne considère que la première adresse renvoyée par getaddrinfo */
  if ((s=socket(resol->ai_family,resol->ai_socktype, resol->ai_protocol))<0) {
    perror("allocation de socket");
    exit(3);
  }
  fprintf(stderr,"le n° de la socket est : %i\n",s);

  /* Connexion */
  fprintf(stderr,"Essai de connexion à %s (%s) sur le port %s\n\n",
	  hote,ip,port);
	if(ipv4_6 == 0){
		if (connect(s,resol->ai_addr,sizeof(struct sockaddr_in))<0) {
		  perror("connexion");
		  exit(4);
		}
	}
	else
		if (connect(s,resol->ai_addr,sizeof(struct sockaddr_in6))<0) {
		  perror("connexion");
		  exit(4);
		}
  freeaddrinfo(resol); /* /!\ Libération mémoire */

  /* Session */
  char tampon[MAXLIGNE + 3]; /* tampons pour les communications */
  ssize_t lu;
  int fini=0;
  while( 1 ) { 
    /* Jusqu'à fermeture de la socket (ou de stdin)     */
    /* recopier à l'écran ce qui est lu dans la socket  */
    /* recopier dans la socket ce qui est lu dans stdin */

    /* réception des données */
    lu = recv(1,tampon,MAXLIGNE,0); /* bloquant */
    
    if (lu == 0 ) {
      fprintf(stderr,"Connexion terminée par l'hôte distant\n");
      break; /* On sort de la boucle infinie */
    }
    tampon[lu] = '\0';
    printf("reçu: %s",tampon);
    
    while(lu==MAXLIGNE){
    	lu = recv(s,tampon,MAXLIGNE,MSG_DONTWAIT);
      tampon[lu] = '\0';
    	printf("%s",tampon);
    }
    
    if ( fini == 1 )
      break;  /* on sort de la boucle infinie*/
    
    /* recopier dans la socket ce qui est entré au clavier */    
    if ( fgets(tampon,MAXLIGNE - 2,stdin) == NULL ){/* entrée standard fermée */
      fini=1;
      fprintf(stderr,"Connexion terminée !!\n");
      fprintf(stderr,"Hôte distant informé...\n");
      shutdown(s, SHUT_WR); /* terminaison explicite de la socket 
			     dans le sens client -> serveur */
      /* On ne sort pas de la boucle tout de suite ... */
    }else{   /* envoi des données */
      send(s,tampon,strlen(tampon),0);
    }
  } 
  /* Destruction de la socket */
  close(s);

  fprintf(stderr,"Fin de la session.\n");
}




void ext-out()
{
  int s,n; /* descripteurs de socket */
  int len,on; /* utilitaires divers */
  struct addrinfo * resol; /* résolution */
  struct addrinfo indic = {AI_PASSIVE, /* Toute interface */
                           PF_INET6,SOCK_STREAM,0, /* IP mode connecté */
                           0,NULL,NULL,NULL};
  struct sockaddr_in6 client; /* adresse de socket du client */
  char * port ; /* Port pour le service */
  int err; /* code d'erreur */
  pid_t pidFiston;
    
  port= "123"; fprintf(stderr,"Ecoute sur le port %s\n",port);
  err = getaddrinfo(NULL,port,&indic,&resol); 
  if (err<0){
    fprintf(stderr,"Résolution: %s\n",gai_strerror(err));
    exit(2);
  }

  /* Création de la socket, de type TCP / IP */
  if ((s=socket(resol->ai_family,resol->ai_socktype,resol->ai_protocol))<0) {
    perror("allocation de socket");
    exit(3);
  }
  fprintf(stderr,"le n° de la socket est : %i\n",s);

  /* On rend le port réutilisable rapidement /!\ */
  on = 1;
  if (setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0) {
    perror("option socket");
    exit(4);
  }
  fprintf(stderr,"Option(s) OK!\n");

// acceptation des 2 ip
int x = 0;
setsockopt(s,IPPROTO_IPV6,IPV6_V6ONLY,&x,sizeof(x));

  /* Association de la socket s à l'adresse obtenue par résolution */
  if (bind(s,resol->ai_addr,sizeof(struct sockaddr_in6))<0) {
    perror("bind");
    exit(5);
  }
  freeaddrinfo(resol); /* /!\ Libération mémoire */
  fprintf(stderr,"bind!\n");

  /* la socket est prête à recevoir */
  if (listen(s,SOMAXCONN)<0) {
    perror("listen");
    exit(6);
  }
  fprintf(stderr,"listen!\n");

  while(1) {
    /* attendre et gérer indéfiniment les connexions entrantes */
    len=sizeof(struct sockaddr_in6);
    do{
		  if( (n=accept(s,(struct sockaddr *)&client,(socklen_t*)&len)) < 0 ) {
		    perror("accept");
		    exit(7);
		  }
			pidFiston = fork();
    }while(pidFiston !=0);
    
    /* Nom réseau du client */
    char hotec[NI_MAXHOST];  char portc[NI_MAXSERV];
    err = getnameinfo((struct sockaddr*)&client,len,hotec,NI_MAXHOST,portc,NI_MAXSERV,0);
    if (err < 0 ){
      fprintf(stderr,"résolution client (%i): %s\n",n,gai_strerror(err));
    }else{ 
      fprintf(stderr,"accept! (%i) ip=%s port=%s\n",n,hotec,portc);
    }
    /* traitement */
    ext-out(n,hotec,portc);
  }
  return;
}

/* echo des messages reçus (le tout via le descripteur f) */
void ext-outmessage(int f, char* hote, char* port)
{
  ssize_t lu; /* nb d'octets reçus */
  char msg[MAXLIGNE+1]; /* tampons pour les communications */ 
  char tampon[MAXLIGNE+1]; 
  int pid = getpid(); /* pid du processus */
  int compteur=0;
  
  /* message d'accueil */
  printf("Bonjour %s! (vous utilisez le port %s)\n",hote,port);
  
  do { /* Faire echo et logguer */
    lu = recv(f,tampon,MAXLIGNE,0);
    if (lu > 0 )
      {
        compteur++;
        tampon[lu] = '\0';
        /* log */
        fprintf(stderr,"[%s:%s](%i): %3i :%s",hote,port,pid,compteur,tampon);
        printf("> %s",tampon);
      } else {
        break;
      }
  } while ( 1 );
       
  /* le correspondant a quitté */
  close(f);
  fprintf(stderr,"[%s:%s](%i): Terminé.\n",hote,port,pid);
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h> 
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include "iftun.h"

int tun_alloc(char *dev)
{
  struct ifreq ifr;
  int fd, err;

  if( (fd = open("/dev/net/tun", O_RDWR)) < 0 ){
    perror("alloc tun");
    exit(1);
  }

  memset(&ifr, 0, sizeof(ifr));

  /* Flags: IFF_TUN   - TUN device (no Ethernet headers) 
   *        IFF_TAP   - TAP device  
   *
   *        IFF_NO_PI - Do not provide packet information  
   */ 
  ifr.ifr_flags = IFF_TUN | IFF_NO_PI; 
  if( *dev )
    strncpy(ifr.ifr_name, dev, IFNAMSIZ);

  if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
    close(fd);
    return err;
  }
  strcpy(dev, ifr.ifr_name);
  return fd;
}      

void recup_paquet(int src,int dst){
	char buffer[1024];
	ssize_t r;
	
	if(read(src, buffer, 1024) <0) return; 
	
	if(write(dst, buffer, 1024)<0)return;  
	printf("\n\n");
}


/*
int main (int argc, char** argv){

	char nomtun[128];
	strcpy(nomtun, argv[1]);

	int fd = tun_alloc(nomtun);
	if(fd<0)return 1;	

	while(1){recup_paquet(fd,1);}

  return 0;
}

*/

#ifndef IFTUN_H
#define IFTUN_H

int tun_alloc(char *dev,char* options);
void recup_paquet(int src,int dst);

#endif

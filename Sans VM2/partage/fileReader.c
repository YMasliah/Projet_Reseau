#include<stdio.h>

int getConf(char *filePath, char *tun, char *inip, char *inport, char *options, char *outip, char* outport)
{
  int fd;
  if((fd = fopen(filePath)) < 0)
  {
    perror("File not found");
    exit(2);
  }





  close(fd);
}

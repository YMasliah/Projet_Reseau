#include<stdio.h>
#include<stdlib.h>

void readConf(char *filePath, char *tun, char *inip, char *inport, char *options, char *outip, char* outport)
{
  FILE *fd;

  if((fd = fopen(filePath, "r")) < 0)
  {
    perror("File not found");
    exit(2);
  }

  char buff[50];
  
  fgets(buff, 50, fd);
  if((fgets(buff, 50, fd)) && (sscanf(buff, "  tun=%s", tun) < 0))
  {
    perror("cannot find tun configuration");
    exit(3);
  }
  
  if((fgets(buff, 50, fd)) && (fscanf(fd, "  inip=%s", inip) < 0))
  {
    perror("cannot find ipin configuration");
    exit(4);
  }

  if((fgets(buff, 50, fd)) && (fscanf(fd, "  inport=%s", inport) < 0))
  {
    perror("cannot find inport configuration");
    exit(5);
  }

  if((fgets(buff, 50, fd)) && (fscanf(fd, "  options=%s", options) < 0))
  {
    perror("cannot find options configuration");
    exit(6);
  }

  fgets(buff, 50, fd);

  if((fgets(buff, 50, fd)) && (fscanf(fd, "  outip=%s", outip) < 0))
  {
    perror("cannot find outin configuration");
    exit(7);
  }

  if((fgets(buff, 50, fd)) && (fscanf(fd, "  outport=%s", outport) < 0))
  {
    perror("cannot find outport configuration");
    exit(8);
  }

  close(fd);
}

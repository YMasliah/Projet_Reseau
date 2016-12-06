#include "conf_manager.h"

void readConf(char *filePath, char *tun, char *inip, char *inport, char *options, char *outip, char* outport, char *netin, char *netout)
{
	FILE *fd;

	if((fd = fopen(filePath, "r")) < 0)
	{
		perror("File not found");
		exit(2);
	}
  
	char buff[50];

	fgets(buff, 50, fd);
	if((fgets(buff, 50, fd)) && (sscanf(buff, "tun=%s", tun) < 0))
	{
		perror("cannot find tun configuration");
		exit(3);
	}

	if((fgets(buff, 50, fd)) && (fscanf(fd, "inip=%s", inip) < 0))
	{
		perror("cannot find ipin configuration");
		exit(4);
	}

	if((fgets(buff, 50, fd)) && (fscanf(fd, "inport=%s", inport) < 0))
	{
		perror("cannot find inport configuration");
		exit(5);
	}

	if((fgets(buff, 50, fd)) && (fscanf(fd, "options=%s", options) < 0))
	{
		perror("cannot find options configuration");
		exit(6);
	}

	fgets(buff, 50, fd);

	if((fgets(buff, 50, fd)) && (fscanf(fd, "outip=%s", outip) < 0))
	{
		perror("cannot find outin configuration");
		exit(7);
	}

	if((fgets(buff, 50, fd)) && (fscanf(fd, "outport=%s", outport) < 0))
	{
		perror("cannot find outport configuration");
	exit(8);
	}

	fgets(buff, 50, fd);

	if((fgets(buff, 50, fd)) && (fscanf(fd, "netin=%s", netin) < 0))
	{
		perror("cannot find netin configuration");
		exit(7);
	}

	if((fgets(buff, 50, fd)) && (fscanf(fd, "netout=%s", netout) < 0))
	{
		perror("cannot find netout configuration");
		exit(8);
	}

	fclose(fd);
}


void writeScript(char *fileName, char *nomTun, char *ipTun, char *netin, char *netout)
{
	FILE *out = fopen(fileName, "w");
	fprintf(out, "sudo ip link set %s up\n", nomTun);
	fprintf(out, "sudo ip addr add %s dev %s\n", ipTun, nomTun);
	fprintf(out, "\n");
	fprintf(out, "sudo ip route add %s via %s\n", netout, ipTun);
	fprintf(out, "sudo ip route add %s via %s\n", netin, ipTun);
	fprintf(out, "sudo sysctl -w net.ipv4.ip_forward=1\n");
	fclose(out);
}

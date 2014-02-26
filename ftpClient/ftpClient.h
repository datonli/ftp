#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <fcntl.h>

#define SERVERPORT 8010
#define MAXLEN 1024
#define MAXORDER 32


int dealorderPRT(char * port);
int connect2server(int sockfd,struct sockaddr_in addr,char * server_ip_address);
char* getusefulmsg(char * ordermsg);

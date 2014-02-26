#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>

#define SERVERPORT 8010
#define MAXLEN 1024
#define MAXORDER 32

void sendmsg2server(int sockfd,char * sendmsg);
int dealorderPRT(char * msg,char * portmsg);
int connect2server(int sockfd,struct sockaddr_in addr,char * server_ip_address);
char* getusefulmsg(char * ordermsg);

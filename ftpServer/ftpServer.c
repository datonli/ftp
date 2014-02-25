/*
 * ftpServer.c
 * 服务器程序
 * Daniel.Lee
 * 2014-2-25
 *
 * */

#include "ftpServer.h"

int main(int argc,char * argv[])
{
        int sockfd,newsockfd,fd;
        char sendmsg[MAXLEN];
        char getmsg[MAXLEN];
        struct sockaddr_in serveraddr;
        struct sockaddr_in clientaddr;
        int clientport,sockaddrlen;
	
	sockfd = listen2client(sockfd,clientaddr,serveraddr);
	
	printf("listenning...\n");
	
LOOP:   if((newsockfd = accept(sockfd,(struct sockaddr *)&clientaddr,&sockaddrlen)) != -1)
        {
                printf("new socket fd is : %d \n ",newsockfd);
		char * clientipaddr = inet_ntoa(clientaddr.sin_addr.s_addr);
		printf("client IP is : %s \n",clientipaddr);
                int pid;
                if((pid = fork()) > 0)
                {
                        goto LOOP;
                }
                else if(pid == 0)
                {
                         int recvnumbytes ;
                         if((recvnumbytes = recv(newsockfd , getmsg, MAXLEN, 0)) == -1)
                         {
                                 printf("recv msg error\n");
                         }
                         else
                         {
				char preordermsg[4];
				char *ordermsg = getmsg;
		                for(int i = 0; i <= 2; i++)
                		        preordermsg[i] = ordermsg[i];
                		preordermsg[3] = '\0';
				printf("msg is %s \n",ordermsg);
				sleep(5);
				if(strcmp(preordermsg,"PRT") == 0)
		                {
                		//	char * port = getusefulmsg(ordermsg);
					newsockfd = connect2client(newsockfd,clientaddr,clientipaddr,ordermsg);						
					printf("PRT already done\n");
				}
		                else if(strcmp(preordermsg,"DIR") == 0)
                		{
					
                		}	
		                else if(strcmp(preordermsg,"GET") == 0)
                		{

                		}
		                else if(strcmp(preordermsg,"BYE") == 0)
                		{

                		}
                		else
                		{
		                        printf("Wrong order!\n");
				}

                                /*
				getmsg[recvnumbytes] = '/0';
                                printf("recvnumbytes is %d ,\nrecvmsg is : %s \n",recvnumbytes,getmsg);
                                if((fd = open("/home/Daniel.Lee/ftp/testNetwork/shello.txt",O_WRONLY | O_CREAT)) == -1)
                                {
                                         printf("open file failed!\n");
                                         exit(0);
                                }
                                int writenumbytes;
                                if((writenumbytes = write(fd,getmsg,strlen(getmsg))) == -1)
                                {
                                         printf("write file failed!\n");
                                         exit(0);
                                }
                                printf("writenumbytes is : %d\n",writenumbytes);
                                close(fd);
				**/

                        }

                }
                else
                {
                        printf("fork failed\n");
                        exit(0);
                }
                        close(newsockfd);
        }


}


int connect2client(int sockfd,struct sockaddr_in addr,char * clientaddr,char * msg)
{
        if((sockfd = socket(AF_INET,SOCK_STREAM,0))<0)
        {
                printf("create socket failed\n");
                exit(0);
        }

        bzero(&addr,sizeof(struct sockaddr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(clientaddr);
	addr.sin_port = htons(atoi(msg+3));
	//addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        printf("Conneting to host (IP is : %s)\n",clientaddr);
        if( (connect(sockfd,(struct sockaddr *) &addr, sizeof(struct sockaddr))) == -1)
        {
                printf("connect failed!!!\n");
                close(sockfd);
                exit(0);
        }
        return sockfd;

}						

char* getusefulmsg(char * ordermsg)
{
        int msglen = strlen(ordermsg) - 4;
        char usefulmsg[msglen];
        for(int i = 0; i < msglen ; i ++)
                usefulmsg[i] = ordermsg[i];
        return usefulmsg;
}

int listen2client(int sockfd ,struct sockaddr_in clientaddr, struct sockaddr_in serveraddr)
{
	if((sockfd = socket(AF_INET,SOCK_STREAM,0) ) < 0)
        {
                printf("creat socket failed\n");
                exit(0);
        }

        bzero(&clientaddr,sizeof(struct sockaddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(SERVERPORT);
        serveraddr.sin_addr.s_addr = INADDR_ANY;

        if(bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr )) < 0)
        {
                printf("bind failed!\n");
                close(sockfd);
                exit(0);
        }
        if(listen(sockfd,5)<0)
        {
                printf("listen failed\n");
                close(sockfd);
                exit(0);
        }
	return sockfd;
}

/*
 * ftpServer.c
 * 服务器程序
 * Daniel.Lee
 * 2014-2-25
 *
 * */

#include "ftpServer.h"
char * serverport;
int main(int argc,char * argv[])
{
        int sockfd,newsockfd,fd,datasockfd;
	int recvnumbytes = -1;
        char sendmsg[MAXLEN];
        char getmsg[MAXLEN];
        struct sockaddr_in serveraddr;
        struct sockaddr_in clientaddr;
        int clientport,sockaddrlen;
	serverport = argv[1];	
	sockfd = listen2client(sockfd,clientaddr,serveraddr);
	
	printf("listenning...\n");
	
LOOP:   if((newsockfd = accept(sockfd,(struct sockaddr *)&clientaddr,&sockaddrlen)) != -1)
        {
		struct in_addr addr;
		memcpy(&addr,&clientaddr.sin_addr.s_addr,4);
		char * clientipaddr = inet_ntoa(addr);
		printf("IP is : %s\n",clientipaddr);
                int pid;
                if((pid = fork()) > 0)
                {
			goto LOOP;
	        }
                else if(pid == 0)
                {
			char * ordermsg;
                        int recvnumbytes ;
		        sleep(5);
LOOP_RECV:              if((recvnumbytes = recv(newsockfd , getmsg, MAXLEN, 0)) == -1)
                        {
                                 printf("recv msg error\n");
				 goto LOOP_RECV;
                        }
			
			//由于接收到的信息是类似于：PRT8000这样的格式的，所以需要对这些字符串进行一些处理之后才能够使用！！！	
			char preordermsg[4];
			ordermsg = getmsg;
			for(int i = 0; i <= 2; i++)
                	        preordermsg[i] = ordermsg[i];
                	preordermsg[3] = '\0';
			printf("msg is %s \n",ordermsg);
			if(strcmp(preordermsg,"PRT") == 0)
		        {
				datasockfd = connect2client(newsockfd,clientaddr,clientipaddr,ordermsg);
				printf("PRT already done\n");
				goto LOOP_RECV;
			}
		        else if(strcmp(preordermsg,"DIR") == 0)
                	{
				int status;
				/*int msglen = strlen(ordermsg) ;
				char cdorder[MAXORDER] ;
				//for(int i = 3; i <= msglen; i ++)
				//	cdorder[i-3] = ordermsg[i];
				ordermsg[0] = 'c';
				ordermsg[1] = 'd';
				ordermsg[2] = ' ';
				if((status = system(ordermsg)) == -1)
				{
				        printf("dir error\n");
                                        exit(0);
				}
				**/
				if((status = system("dir > systemp.txt")) == -1)
				{
					printf("dir error\n");
					exit(0);
				}		
				if((fd = open("./systemp.txt",O_RDONLY)) == -1)
			        {
			                printf("open file failed!\n");
			                exit(0);
			        }       
			        if((status = read(fd,sendmsg,MAXLEN)) == -1)
			        {		
			                printf("read file failed!\n");
			                exit(0);
			        }
			        printf("readbytenum is : %d \n",status);
			        if( send(datasockfd,sendmsg,strlen(sendmsg),0) < 0)
			        {
			                printf("send failed");
			                exit(0);
			        }
			        printf("send msg is : %s \n",sendmsg);  
				close(fd);
				if((status = system("rm -rf systemp.txt")) == -1)
				{
					printf("rm temp failed\n");
					exit(0);
				}
				goto LOOP_RECV;				
                	}	
		        else if(strcmp(preordermsg,"GET") == 0)
                	{
				int status;
				char arguemsg[MAXORDER];
				for(int i = 0; i <= strlen(ordermsg)-3; i ++)
                              		arguemsg[i] = ordermsg[i+3];
				
                              	printf("GET %s ... \n",arguemsg);
				if((fd = open(arguemsg,O_RDONLY)) == -1)
			        {
			                printf("open file failed!\n");
			                exit(0);
			        }      
				memset(sendmsg,0,strlen(sendmsg)+1); 
			        if((status = read(fd,sendmsg,MAXLEN)) == -1)
			        {
			                printf("read file failed!\n");
			                exit(0);
			        }
			        printf("readbytenum is : %d \n",status);
			        if( send(datasockfd,sendmsg,strlen(sendmsg),0) < 0)
			        {
			                printf("send failed");
			                exit(0);
			        }
			        printf("send msg is : %s \n",sendmsg);  
				close(fd);	
				goto LOOP_RECV;
                	}
		        else if(strcmp(preordermsg,"BYE") == 0)
                	{
                       		close(newsockfd);
				close(datasockfd);
               		}
               		else
               		{
	                        printf("Wrong order!\n");
			}
			recvnumbytes = -1;
               }
                else
                {
                        printf("fork failed\n");
                        exit(0);
                }
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
        printf("Conneting to host (IP is : %s)\n",clientaddr);
        if( (connect(sockfd,(struct sockaddr *) &addr, sizeof(struct sockaddr))) == -1)
        {
                printf("connect failed!!!\n");
                close(sockfd);
                exit(0);
        }
        return sockfd;

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
        //serveraddr.sin_port = htons(SERVERPORT);
        serveraddr.sin_port = htons(atoi(serverport));
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

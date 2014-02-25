
/*
 *客户端程序  ftpClient.c 
 *2014-2-24 
 *by Daniel.Lee
 *
 * */

#include "ftpClient.h"

int main(int argc,char * argv[])
{
	int ordersockfd,datasockfd ,status,bye = 0;
        struct sockaddr_in serveraddr;
        struct sockaddr_in clientaddr;
        char sendmsg[MAXLEN];
        char recvmsg[MAXLEN];
	char ordermsg[MAXORDER];
	if(argc != 2)
	{
		printf("Wrong format!\nYou should type like that:\n    ./ftpClient.o server_ip_address\n");
	}
	
	ordersockfd = connect2server(ordersockfd,serveraddr,argv[1]);
	
	printf("Conneted.\n");
LOOP:	printf("Please enter your order:\n");
	printf("1.PRT port_number\n");
        printf("2.DIR\n");
        printf("3.GET filename\n");
        printf("4.BYE\n");
	
	while(bye != 1)
	{	
		scanf("%s",ordermsg);
		sendmsg2server(ordersockfd,ordermsg);
		char preordermsg[4] = {};
		for(int i = 0; i <=2; i++)
			preordermsg[i] = ordermsg[i];
		preordermsg[3] = '\0';
		if(strcmp(preordermsg,"PRT") == 0)
		{
		//	char * port = getusefulmsg(ordermsg);
			datasockfd = dealorderPRT(ordermsg);	
		}
		else if(strcmp(preordermsg,"DIR") == 0)
		{

		}
		else if(strcmp(preordermsg,"GET") == 0)
		{

		}
		else if(strcmp(preordermsg,"BYE") == 0)
		{
			//all close()
			bye = 1;
		}
		else
		{
			printf("Wrong order!\n");
			goto LOOP;
		}		


		printf("Enter your order:\n");
	}
}
		

void sendmsg2server(int sockfd,char * sendmsg)
{
	if( send(sockfd,sendmsg,strlen(sendmsg),0) < 0)
        {
                printf("send failed");
                exit(0);
        }
        printf("send msg is : %s \n",sendmsg);  
}

char* getusefulmsg(char * ordermsg)
{
	int msglen = strlen(ordermsg) - 4;
	char usefulmsg[msglen + 1] ;
	for(int i = 0; i < msglen ; i ++)
		usefulmsg[i] = ordermsg[i+4];
	return usefulmsg;
}

/*
 *处理PRT命令，步骤：
	1）自己先使用这个端口开一个新的socket，并且进行监听链接这个端口的机器；
	2）将port发送到server，让server也开一个新的socket，链接到本机；这时，考虑要将PRT命令也发到server上，让它也判断一次要进行PRT开socket链接这一步！！！
	3）链接成功的话，就完成了这一步了。这是开通一个专门用来传输数据的数据链路。
 *
 * */
int dealorderPRT(char * msg)
{
	int sockfd;
	struct sockaddr_in serveraddr,clientaddr;
	if((sockfd = socket(AF_INET,SOCK_STREAM,0) ) < 0)
        {
                printf("creat socket failed\n");
                exit(0);
        }

        bzero(&clientaddr,sizeof(struct sockaddr));
        serveraddr.sin_family = AF_INET;
	int port = atoi(msg+3);
	printf("port is %d\n",port);
	serveraddr.sin_port = htons(port);
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
        int sockaddrlen = sizeof(clientaddr);
	if((sockfd = accept(sockfd,(struct sockaddr *)&clientaddr,&sockaddrlen)) != -1)
	{
		printf("Connected by Host(IP is : %s)\n",inet_ntoa(clientaddr.sin_addr.s_addr));		
	}
	return sockfd;
}

int connect2server(int sockfd,struct sockaddr_in addr,char * server_ip_address)
{
	if((sockfd = socket(AF_INET,SOCK_STREAM,0))<0)
        {
                printf("create socket failed\n");
                exit(0);
        }

        bzero(&addr,sizeof(struct sockaddr));
        addr.sin_family = AF_INET;
        addr.sin_port =htons(SERVERPORT);
        addr.sin_addr.s_addr = inet_addr(server_ip_address);
        
        printf("Conneting to host (IP is : %s)\n",server_ip_address);
        if( (connect(sockfd,(struct sockaddr *) &addr, sizeof(struct sockaddr))) == -1)
        {
                printf("connect failed\n");
                close(sockfd);
                exit(0);
        }
	return sockfd;
}

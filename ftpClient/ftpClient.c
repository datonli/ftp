
/*
 *客户端程序  ftpClient.c 
 *2014-2-24 
 *by Daniel.Lee
 *
 * */

#include "ftpClient.h"
char * serverport;
int main(int argc,char * argv[])
{
	int ordersockfd,datasockfd ,status,bye = 0;
        struct sockaddr_in serveraddr;
        struct sockaddr_in clientaddr;
        char sendmsg[MAXLEN];
        char recvmsg[MAXLEN];
	char ordermsg[MAXORDER];
	char arguemsg[MAXORDER];
	serverport = argv[2];
	if(argc != 3)
	{
		printf("Wrong format!\nYou should type like that:\n    ./ftpClient.o server_ip_address\n");
	}
	
	ordersockfd = connect2server(ordersockfd,serveraddr,argv[1]);
	
	printf("Conneted.\n");
LOOP:	printf("Please enter your order:\n");
	printf("1.PRT port_number\n");
        printf("2.DIR pathname\n");
        printf("3.GET filename\n");
        printf("4.BYE BYE\n");
	
	while(bye != 1)
	{	
		scanf("%s %s",ordermsg,arguemsg);
		printf("ordermsg is : %s , arguemsg is %s\n",ordermsg,arguemsg);
		int ordermsglen = strlen(ordermsg),arguemsglen = strlen(arguemsg);
		char send2servermsg[ordermsglen + arguemsglen+1];
		for(int i = 0; i <= ordermsglen -1 ; i ++)
			send2servermsg[i] = ordermsg[i];
		for(int i = 0; i <= arguemsglen - 1; i++)
			send2servermsg[ordermsglen+i] = arguemsg[i];
		send2servermsg[ordermsglen + arguemsglen] = '\0';
		sendmsg2server(ordersockfd,send2servermsg);
		if(strcmp(ordermsg,"PRT") == 0)
		{
			datasockfd = dealorderPRT(ordermsg,arguemsg);	
		}
		else if(strcmp(ordermsg,"DIR") == 0)
		{
			int recvnumbytes;
			if((recvnumbytes = recv(ordersockfd , recvmsg, MAXLEN, 0)) == -1)
		        {
               			printf("recv failed\n");
		                exit(0);
		        }
			printf("DIR is :\n%s\n",recvmsg);
		}
		else if(strcmp(ordermsg,"GET") == 0)
		{

		}
		else if(strcmp(ordermsg,"BYE") == 0)
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

/*
 *处理PRT命令，步骤：
	1）自己先使用这个端口开一个新的socket，并且进行监听链接这个端口的机器；
	2）将port发送到server，让server也开一个新的socket，链接到本机；这时，考虑要将PRT命令也发到server上，让它也判断一次要进行PRT开socket链接这一步！！！
	3）链接成功的话，就完成了这一步了。这是开通一个专门用来传输数据的数据链路。
 *
 * */
int dealorderPRT(char * msg,char * portmsg)
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
	int port = atoi(portmsg);
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
		struct in_addr addr;
                memcpy(&addr,&clientaddr.sin_addr.s_addr,4);
                char * clientipaddr = inet_ntoa(addr);

		printf("Connected by Host(IP is : %s)\n",clientipaddr);		
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
        //addr.sin_port =htons(SERVERPORT);
        addr.sin_port =htons(atoi(serverport));
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

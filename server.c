#include<stdio.h>
#include<ctype.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include "deletefile.h"

char * makeresponse(int sockfd, int choice){
	switch (choice){
	case 1:
	{
		char response[1024] = { 0 };
		printDetails(&currentuser,response);
		write(sockfd, response, strlen(response));
		break;
	}
	case 2:{
		int size = 0, blockcount = 0;
		char filename[32];
		memset(&filename, 0, sizeof(filename));
		read(sockfd, filename, sizeof(filename));

		printf("%s", filename);

		char buff[32];
		memset(&buff, 0, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		size = atoi(buff);
		printf("%d", size);
		
		blockcount = (size / BLOCKSIZE) + (size%BLOCKSIZE ? 1 : 0);
		
		struct block *blocks = (struct block*)malloc(sizeof(struct block)*blockcount);
		memset(blocks, 0, sizeof(struct block)*blockcount);
		
		int i = 0,bytes=0;
		while (i < blockcount){
			bytes=read(sockfd, (&blocks[i]), sizeof(struct block));
			//printf("Successfully received %d bytes", bytes);
			i++;
		}

		addfile1(filename, blocks, blockcount, size);
		break;
	}
	case 3:
	{
		char buff[32];
		int filenum;
		read(sockfd, buff, sizeof(buff));
		filenum = atoi(buff);
		write(1, buff, 2);

		int size = currentuser.fileset[filenum - 1].size;
		int blockcount = (size / BLOCKSIZE) + (size%BLOCKSIZE ? 1 : 0);
		struct block *blocks = (struct block *)malloc(blockcount*sizeof(struct block));
		downloadfile2(blocks,currentuser.fileset[filenum - 1].offset, currentuser.fileset[filenum - 1].size);
		printf("blockcount:%d", blockcount);
		
		memset(&buff, 0, sizeof(buff));
		sprintf(buff, "%d", size, blockcount);
		write(sockfd, buff, sizeof(buff));

		int h = 0;
		while (h < blockcount)
		{
			write(sockfd, (&blocks[h]), sizeof(struct block));
			h++;
		}
		//struct block b = { "" };
		//int i = 0;
		//while ((i + 1)*BLOCKSIZE<size){
		//	fwrite(&blocks[i].b, BLOCKSIZE, 1, downloaded);
		//	i++;
		//}
		//int le = size%BLOCKSIZE;
		//fwrite(&blocks[i].b, le, 1, downloaded);
		///*int k = 0;
		//char buff = 0;
		//while (i < size%BLOCKSIZE){
		//fwrite(&blocks[i].b[k], sizeof(char), 1, downloaded);
		//k++;
		//}*/
		//fclose(downloaded);
		//printf("Successfully downloaded");
		//downloadfile(currentuser.fileset[filenum - 1].offset, currentuser.fileset[filenum - 1].size);
		break;
	}
	case 4:
	{
		char buff[32];
		memset(&buff, 0, sizeof(buff));
		fflush(stdin);
		read(sockfd, buff, sizeof(buff));
		int num = atoi(buff);
		printf("received filenum : %d", num);
		deletefile(num);
		break;
	}
	case 5:
		exit(0);
	}
}
int main()
{
int sockfd,newsockfd,n,len;
char buff[512];
/*sockaddr_in is predefined structure*/
struct sockaddr_in ser_addr,cli_addr;
/*Creating a connection-Oriented server socket*/
if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
{
    perror("socket error\n");
    exit(1);
}
/*Initializing the socket with family,port number,ipaddress*/
ser_addr.sin_family=AF_INET;
ser_addr.sin_port=htons(8810);
ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
/*Binding the structure to the server socket*/
if(bind(sockfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr))<0)
{
    perror("bind error");
    exit(1);
}
/*Listen to indicate that it is ready to receive connections*/
listen(sockfd,5);
len=sizeof(cli_addr);
printf("server is waiting for connection .....\n");
/*accept takes the first connection request*/
if((newsockfd=accept(sockfd,(struct sockaddr*)&cli_addr,&len))<0)
{
    perror("accept error");
    exit(1);
}
fflush(stdout);
//printf("IP addresss of client %s",inet_ntoa(cli_addr.sin_addr));
//printf("port no. of server is %d",ntohs(ser_addr.sin_port));
printf("server is connected to the client......\n");
fflush(stdout);
    
memset(&buff, 0, sizeof(buff));
read(newsockfd, buff, sizeof(buff));

int id = atoi(buff);
init(id,newsockfd);
while(1){/*Reading a message from Client*/
	
	memset(&buff, 0, sizeof(buff));

	read(newsockfd, buff, sizeof(buff));

	printf("\nclient :\t %s", buff);
	
	if (bcmp(buff, "exit", 4) == 0)
	{
		printf("\n server is closing the connection .\n");
		exit(1);
	}
	int choice = atoi(buff);
	
	/*Sending a message to Client*/
	
	makeresponse(newsockfd, choice);
	//write(newsockfd, "got something here", 32);
}
/*Closing the socket*/
close(sockfd);
return 0;
}

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#define BLOCKSIZE 512

struct block{
	char b[512];
};

int main()
{
int sockfd,n;
char buff1[512],buff2[512];
/*sockaddr_in is predefined structure*/
struct sockaddr_in serv_addr;
/*Creating a connection-Oriented client socket*/
if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
{
    perror("socket error\n");
    exit(1);
}
/*Initializing the server socket with family,port number,ipaddress*/
serv_addr.sin_family=AF_INET;
serv_addr.sin_port=htons(8810);
serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");;
/*connect system call to establish a connection with a server*/
if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
{
    perror("connect error");
    exit(1);
}
memset(&buff1, 0, sizeof(buff1));

printf("Enter User ID : ");
scanf("%s", buff1);
write(sockfd, buff1, sizeof(buff1));

int success=-1;
read(sockfd,&success,sizeof(int));
if(!success){
	char response[256]={0};
	printf("No users with the userid... Do you want to create a new user?(Y\\N)\n");
	scanf("%s",response);
	// memset(&response,0,sizeof(response));
	if(response[0]=='y'||response[0]=='Y'){
		//send if you want to create a user or not (y/n)
		write(sockfd,&response,sizeof(response));
		memset(&response,0,sizeof(response));
		fflush(stdout);
		printf("Enter username: ");
		//read name from user
		scanf("%s",response);
		write(sockfd,&response,sizeof(response));
		memset(&response,0,sizeof(response));

		read(sockfd,&response,sizeof(response));
		printf("%s",response);
	}
}
while(1)
{
    memset(&buff1,0,sizeof(buff1));
    memset(&buff2,0,sizeof(buff2));

	

	int choice = 0;
	printf("\n----------------------------------------------------------------------------------------------------------------\n");
	printf("1. Show All Files\n");
	printf("2. Add a File\n");
	printf("3. Download a File\n");
	printf("4. Delete a File\n");
	printf("5. Exit\n");
	printf("----------------------------------------------------------------------------------------------------------------\n");
	printf("Enter your choice: ");
	
	
	fflush(stdout);
	//printf("IP addresss of server is  %s",inet_ntoa(serv_addr.sin_addr));
	/*Reading a message from Keyboard to buff1*/
	n = read(0, buff1, sizeof(buff1));
	printf("\n message sent to the server  :\t %s", buff1);
	fflush(stdout);
	/*Sending a message to Server*/
	write(sockfd, buff1, n);
	if (bcmp(buff1, "exit", 4) == 0)
	{
		printf("\n client  is exiting... .\n");
		exit(1);
	}


	choice = atoi(buff1);

	
	switch (choice){
	case 1:
	{
		printf("Displaying Current User Details\n");
		printf("\n----------------------------------------------------------------------------------------------------------------\n");

		/*Receiving a message from Server*/
		n = read(sockfd, buff2, 512);
		//printf("message received from the server\n");
		fflush(stdout);
		/*Displaying the message on monitor*/
		write(1, buff2, strlen(buff2));
		printf("\n----------------------------------------------------------------------------------------------------------------\n");

		break;
	}
	case 2:
	{
		//code for adding a file
		int size = 0, blockcount = 0;
		char filename[32];

		printf("Enter filename to be uploaded : ");
		scanf("%s", filename);
		write(sockfd, filename, sizeof(filename));

		FILE *temp = fopen(filename, "rb");
		fseek(temp, 0, SEEK_END);
		size = ftell(temp);
		fseek(temp, 0, SEEK_SET);
		
		memset(&filename, 0, sizeof(filename));
		sprintf(filename, "%d", size);
		write(sockfd, filename, sizeof(filename));

		blockcount = (size / BLOCKSIZE) + (size%BLOCKSIZE ? 1 : 0);

		struct block *blocks = (struct block*)malloc(sizeof(struct block)*blockcount);
		memset(blocks, 0, sizeof(struct block)*blockcount);
		
		int i = 0;
		//reading block by block into struct block blocks
		while (i < blockcount){
			fread((&blocks[i])->b, sizeof(struct block), 1, temp);
			i++;
		}

		int h = 0;
		while (h < blockcount)
		{
			write(sockfd, (&blocks[h]), sizeof(struct block));
			h++;
		}
		printf("File Successfully uploaded\n");
		break;
	}
	case 3:
	{
		//code for downloading a file

		//reading file number to be downloaded
		
		
		printf("Enter the file number to be downloaded: ");
		scanf("%s", buff2);
		//send filenumber to server
		write(sockfd, buff2, strlen(buff2));

		//first read SIZE of the file
		memset(&buff2, 0, sizeof(buff2));
		read(sockfd, buff2, sizeof(buff2));
		int size = atoi(buff2);
		printf("Downloading a file of size %d\n", size);

		char asdf[32];
		printf("Enter the filename to save the downloaded file : ");
		scanf("%s",asdf);
		
		//compute number of blocks
		int blockcount = (size / BLOCKSIZE) + (size%BLOCKSIZE ? 1 : 0);
		
		//create so many blocks
		struct block *blocks = (struct block *)malloc(blockcount*sizeof(struct block));
		
		//open the downloaded file
		FILE *downloaded= fopen(asdf, "wb+");
		//get filename
		fseek(downloaded,size,SEEK_SET);
		fseek(downloaded,-1,SEEK_CUR);
		fprintf(downloaded, (char)0 );
		fseek(downloaded,0,SEEK_SET);
		//receive blocks from server
		int v = 0;
		while (v<blockcount){
			int bytes = read(sockfd, (&blocks[v])->b, sizeof(struct block));
			fflush(stdout);
			//printf("Successfully received %d bytes", bytes);
			//write(1, (&blocks[v])->b, sizeof(struct block));
			
			v++;
		}
		v = 0;
		while (v < blockcount - 1){
			fwrite((&blocks[v])->b, sizeof(struct block), 1, downloaded);
			v++;
		}
		fwrite((&blocks[v])->b, size%BLOCKSIZE, 1, downloaded);
		
		printf("File successfully downloaded to \"%s\"",asdf);
		fclose(downloaded);
		break;
	}
	case 4:
	{
		//code for deleting a file
		int num = 0;
		//printDetails(&currentuser);
		char buff[32];
		printf("Enter the filenumber to be deleted : ");
		memset(&buff, 0, sizeof(buff));
		scanf("%s", buff);
		num = atoi(buff);
		printf("Trying to delete file number %d\n", num);
		write(sockfd, buff, sizeof(buff));
		printf("Successfully deleted file number %d\n",num);
		break;
	}
	case 5:
	{
		//code for exiting a program

		exit(0);
	}
	}


	
}
/*Closing the socket*/
close(sockfd);
}

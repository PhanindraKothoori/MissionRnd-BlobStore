#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define BLOCKSIZE 512

FILE *source;
FILE *blob;
FILE *finalblob;
FILE *meta;
FILE *bitmap;

struct file{
	char filename[24];
	int size;
	int offset;
};

struct user{
	char name[24];
	int userid;
	struct file fileset[15];
	int filecount;
};

struct block{
	char b[512];
};

struct user currentuser;

void reset(FILE *fp){
	unsigned int size;
	fseek(fp, 0, SEEK_END);
	size = ftell(blob);
	fseek(fp, 0, SEEK_SET);
	char c = 0;
	for (int i = 0; i < size; i++){
		fwrite(&c, 1, 1, fp);
	}
}

void openfiles(){
	blob = fopen("blobfile.bin", "rb+");
	meta = fopen("meta.bin", "rb+");
	bitmap = fopen("bitvector.bin", "rb+");
}

void closefiles(){
	fclose(blob);
	fclose(meta);
	fclose(bitmap);
}

void init(int id, int sockfd){
	openfiles();
	memset(&currentuser, 0, sizeof(currentuser));
	//capture current user
	
	char ch[3]={0};
	char response[256];
	memset(&response,0,256);
	fseek(meta, 0, SEEK_SET);
	int success=-1;
	while (!(feof(meta))){
		fread(&currentuser, sizeof(currentuser), 1, meta);
		if (currentuser.userid == 0){
			//sending success as false
			success=0;
			write(sockfd,&success,sizeof(success));

			fseek(meta, -(sizeof(currentuser)), SEEK_CUR);
			
			int a=read(sockfd,&response,sizeof(response));

			printf("\n\n%s\n\n", response);

			if (response[0] == 'Y' || response[0] == 'y'){
				//read username
				memset(&response,0,sizeof(response));
				a=read(sockfd,&response,sizeof(response));

				printf("client sent username as %s",response);
				currentuser.userid = (ftell(meta) / sizeof(struct user)) + 1;
				strcpy(currentuser.name,response);

				printf("\nCreating new user...\n");
				fwrite(&currentuser, sizeof(struct user), 1, meta);

				memset(&response,0,sizeof(response));
				sprintf(response,"Successfully Created a user %s with user id %d",currentuser.name,currentuser.userid);
				write(sockfd,&response,sizeof(response));
			}
			else{
				close(sockfd);
			}
			break;
		}
		if (currentuser.userid == id){
			//sending success as true
			success=1;
			write(sockfd,&success,sizeof(success));
			break;
		}
	}
	fclose(meta);
	meta = fopen("meta.bin", "rb+");

}

int getfreeoffset(int required){
	int zeros = 0, count = 0, start=0,j=0;
	char c;
	for (int i = 0; i < 2 * 1024; i++){
		start = i;
		while ( count < required ){
			c = fgetc(bitmap);
			if (c == 0){
				count++;
			}
			else if (c == '1'){
				count = 0;
				break;
			}
			i++;
		}
		if (count == required) return start;

	}
}

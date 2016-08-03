#include "deletefile.h"
#include<stdlib.h>
void main(){
	int id = 0;
	printf("enter uid  : ");
	scanf("%d", &id);
	init(id);
	do{
		int choice = 0;
		printf("\n--------------------------------------------------------\n");
		printf("1. Show All Files\n");
		printf("2. Add a File\n");
		printf("3. Download a File\n");
		printf("4. Delete a File\n");
		printf("5. Exit\n");
		printf("--------------------------------------------------------\n");
		printf("Enter your choice: ");
		scanf("%d", &choice);
		switch (choice){
		case 1:
		{
			printDetails2(&currentuser);
			break;
		}
		case 2:{
			int size = 0, blockcount = 0;
			char filename[32];

			printf("Enter filename to be uploaded : ");
			scanf("%s", filename);

			FILE *temp = fopen(filename, "rb");
			fseek(temp, 0, SEEK_END);
			size = ftell(temp);
			fseek(temp, 0, SEEK_SET);

			blockcount = (size / BLOCKSIZE) + (size%BLOCKSIZE ? 1 : 0);

			struct block *blocks = (struct block*)malloc(sizeof(struct block)*blockcount);
			memset(blocks, 0, sizeof(struct block)*blockcount);

			int i = 0;
			//reading block by block into struct block blocks
			while (i < blockcount){
				fread((&blocks[i])->b, sizeof(struct block), 1, temp);
				i++;
			}

			addfile1(filename, blocks, blockcount, size);
			break;
		}
		case 3:
		{
			int filenum;
			printf("Enter the file number to be downloaded: ");
			scanf("%d", &filenum);

			FILE *downloaded;
			char filename[25];
			//get filename
			printf("Enter the filename to save the downloaded file : ");
			scanf("%s", filename);
			downloaded = fopen(filename, "wb+");

			int size = currentuser.fileset[filenum - 1].size;
			int blockcount = (size / BLOCKSIZE) + (size%BLOCKSIZE ? 1 : 0);
			printf("size:%d\tblockcount:%d", size, blockcount);
			struct block *blocks=(struct block *)malloc(blockcount*sizeof(struct block));
			downloadfile2(blocks,currentuser.fileset[filenum - 1].offset, currentuser.fileset[filenum - 1].size);

			printf("Got the downloaded data into blocks");

			struct block b = { "" };
			int i = 0;
			while ((i + 1)*BLOCKSIZE<size){
				fwrite(&blocks[i].b, BLOCKSIZE, 1, downloaded);
				i++;
			}
			int le = size%BLOCKSIZE;
			fwrite(&blocks[i].b, le, 1, downloaded);
			fclose(downloaded);
			printf("Successfully downloaded");
			break;
		}
		case 4:
		{

			fflush(stdin);
			fflush_unlocked(stdin);
			int n = 0;
			printDetails2(&currentuser);
			printf("Enter the file number to be deleted : ");
			scanf("%d", &n);
			printf("Trying to delete file number %d", n);
			deletefile(n);
			break;
		}
		case 5:
			exit(0);
		}
	} while (1);
}
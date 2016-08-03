#include "list.h"
void downloadfile(int offset, int size){
	FILE *downloaded;
	char filename[25];
	//get filename
	printf("Enter the filename to save the downloaded file : ");
	scanf("%s", filename);
	downloaded = fopen(filename, "wb+");

	fseek(blob, offset*BLOCKSIZE, SEEK_SET);
	
	struct block b = { "" };
	int i = 0;
	while ((i+1)*BLOCKSIZE<size){
		memset(&b, 0, BLOCKSIZE);
		fread(&b, BLOCKSIZE, 1, blob);
		fwrite(&b, BLOCKSIZE, 1, downloaded);
		i++;
	}
	i = 0;
	char buff = 0;
	while (i < size%BLOCKSIZE){
		fread(&buff, sizeof(char), 1, blob);
		fwrite(&buff, sizeof(char), 1, downloaded);
		i++;
	}
	printf("Successfully Downloaded");
	fclose(downloaded);
	closefiles();
	openfiles();
}
struct block* downloadfile1(int offset, int size){
	FILE *downloaded;
	char filename[25];
	//get filename
	//printf("Enter the filename to save the downloaded file : ");
	//scanf("%s", filename);
	//downloaded = fopen(filename, "wb+");
	fseek(blob, offset*BLOCKSIZE, SEEK_SET);

	int blockcount = (size / BLOCKSIZE) + (size%BLOCKSIZE ? 1 : 0);

	struct block *blocks = (struct block*)malloc(sizeof(struct block)*blockcount);
	memset(blocks, 0, sizeof(struct block)*blockcount);

	//reading block-wise into the struct block *blocks
	int i = 0;
	while (i < blockcount-1){
		fread((&blocks[i])->b, sizeof(struct block), 1, blob);
		i++;
	}
	int k = 0, remaining = size%BLOCKSIZE;
	char buff = 0;
	while (k < remaining){
		fread((&blocks[i])->b[k], sizeof(char), 1, blob);
		k++;
	}
	printf("Successfully Copied into blocks");
	fclose(downloaded);
	closefiles();
	openfiles();
	return blocks;
}
void downloadfile2(struct block blocks[], int offset, int size){
	char filename[25];
	//get filename
	//printf("Enter the filename to save the downloaded file : ");
	//scanf("%s", filename);
	//downloaded = fopen(filename, "wb+");
	fseek(blob, offset*BLOCKSIZE, SEEK_SET);

	int blockcount = (size / BLOCKSIZE) + (size%BLOCKSIZE ? 1 : 0);

	//reading block-wise into the struct block *blocks
	int i = 0;
	while (i < blockcount){
		fread((&blocks[i])->b, sizeof(struct block), 1, blob);
		i++;
	}
	/*int k = 0, remaining = size%BLOCKSIZE;
	char buff = 0;
	while (k < remaining){
		fread((&blocks[i])->b[k], sizeof(char), 1, blob);
		k++;
	}*/
	printf("Successfully Copied into blocks");
	closefiles();
	openfiles();
	
}

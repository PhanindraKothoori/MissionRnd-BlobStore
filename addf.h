#include "initialize.h"
int getFreeFilesetIndex(struct user *u){
	int i = 0;
	while ( i<=u->filecount ){
		if (u->fileset[i].size == 0)
			return i;
		i++;
	}
}

void modifyBV(int size,int offset){
	int blocks = (size / BLOCKSIZE) + (size%BLOCKSIZE ? 1 : 0);
	char c = '1';
	fseek(bitmap, offset, SEEK_SET);
	for (int i = 0; i <= blocks; i++){
		fwrite(&c,sizeof(char),1,bitmap);
	}
}

void addfile(char filename[]){
	
	source = fopen(filename, "rb");
	struct file f;
	memset(&f, 0, sizeof(f));
	int size = 0, offset = 0;
	fseek(source, 0, SEEK_END);
	size = ftell(source);
	fseek(source, 0, SEEK_SET);

	//check for the empty location in bin file using bitvector
	offset = getfreeoffset((size / BLOCKSIZE) + (size%BLOCKSIZE ? 1 : 0));

	//insert the new file to the currentuser's fileset
	int index=getFreeFilesetIndex(&currentuser);
	if (index != -1){
		strcpy(currentuser.fileset[index].filename,filename);
		currentuser.fileset[index].size = size;
		currentuser.fileset[index].offset = offset;
		currentuser.filecount++;
	}
	else{
		printf("BLOB is full...\n");
	}

	//add the userdetails in the meta and append this file to the user's fileset
	fseek(meta, (currentuser.userid-1)*sizeof(struct user), SEEK_SET);
	fwrite(&currentuser, sizeof(struct user), 1, meta);

	//writing into the binary file from the offset
	fseek(blob, offset*BLOCKSIZE, SEEK_SET);
	struct block b = { "" };
	unsigned char buff;
	int i = 0;
	while ((i)*BLOCKSIZE<size){
		memset(&b, 0, BLOCKSIZE);
		fread(&b, BLOCKSIZE, 1, source);
		fwrite(&b, sizeof(struct block), 1, blob);
		i++;
	}

	modifyBV(size, offset);
	printf("Successfully Written");

	closefiles();
	openfiles();
}

void addfile1(char filename[], struct block blocks[], int blockcount, int size){
	struct file f;
	memset(&f, 0, sizeof(f));

	//check for the empty location in bin file using bitvector
	int offset = getfreeoffset((size / BLOCKSIZE) + (size%BLOCKSIZE ? 1 : 0));

	//insert the new file to the currentuser's fileset
	int index = getFreeFilesetIndex(&currentuser);
	if (index != -1){
		strcpy(currentuser.fileset[index].filename, filename);
		currentuser.fileset[index].size = size;
		currentuser.fileset[index].offset = offset;
		currentuser.filecount++;
	}
	else{
		printf("BLOB is full...\n");
	}
	//add the userdetails in the meta and append this file to the user's fileset
	fseek(meta, (currentuser.userid - 1)*sizeof(struct user), SEEK_SET);
	fwrite(&currentuser, sizeof(struct user), 1, meta);

	//writing into the binary file from the offset
	fseek(blob, offset*BLOCKSIZE, SEEK_SET);
	struct block b = { "" };
	unsigned char buff;
	int i = 0;
	while (i<blockcount){
		fwrite(blocks[i].b, sizeof(struct block), 1, blob);
		i++;
	}

	modifyBV(size, offset);
	printf("Successfully Written");
	closefiles();
	openfiles();
}
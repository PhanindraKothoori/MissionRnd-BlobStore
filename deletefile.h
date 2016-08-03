//#include "initialize.h"
#include "download.h"
void clearbitvector(int offset, int size){
	fseek(bitmap, offset, SEEK_SET);
	int blocks = (size / BLOCKSIZE) + (size%BLOCKSIZE ? 1 : 0);
	char c = 0;
	while (blocks-- > 0)
	fwrite(&c, sizeof(char), 1, bitmap);
}
void clearblob(int offset, int size){
	fseek(blob, offset, SEEK_SET);
	int blocks = (size / BLOCKSIZE) + (size%BLOCKSIZE ? 1 : 0);
	struct block b = { "" };
	while (blocks-- >= 0)
	fwrite(&b, sizeof(b), 1, blob);
}
void deletefile(int num){
	
	printf("Entered deletefile function with num=%d\n", num);
	//clear the bit vector
	clearbitvector(currentuser.fileset[num - 1].offset, currentuser.fileset[num - 1].size);
	
	//clear blob area
	clearblob(currentuser.fileset[num - 1].offset, currentuser.fileset[num - 1].size);
	
	//reset the fileset of the current user and filecount
	memset(&currentuser.fileset[num - 1], 0, sizeof(struct file));
	currentuser.filecount--;

	//add the userdetails in the meta and refresh the user's fileset
	fseek(meta, (currentuser.userid - 1)*sizeof(struct user), SEEK_SET);
	fwrite(&currentuser, sizeof(struct user), 1, meta);
	closefiles();
	openfiles();
}
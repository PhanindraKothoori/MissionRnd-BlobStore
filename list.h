#include "addf.h"
void printDetails2(struct user *u){
	int count = 0;
	printf("\nCurrent user details\nName: %s\nID: %d\nFile Count: %d\n\nFile Number\tFile Name \n", u->name, u->userid, u->filecount);
	for (int i = 0; count < u->filecount; i++){
		if (u->fileset[i].size == 0){

			continue;
		}
		else{
			printf("\t%2d.     %s\n",i+1, u->fileset[i].filename);
			count++;
		}
	}
}

char * printDetails(struct user *u, char response[]){
	char temp[32];
	int count = 0;
	printf("\nCurrent user details\nName: %s\nID: %d\nFile Count: %d\n\nFile Number\tFile Name \n", u->name, u->userid, u->filecount);
	sprintf(response, "\nCurrent user details\nName: %s\nID: %d\nFile Count: %d\n\nFile Number\tFile Name \n", u->name, u->userid, u->filecount);
	for (int i = 0; count < u->filecount; i++){
		if (u->fileset[i].size == 0){

			continue;
		}
		else{
			printf("\t%2d.     %s\n", i + 1, u->fileset[i].filename);
			sprintf(temp, "\t%2d.     %s\n", i + 1, u->fileset[i].filename);
			strcat(response, temp);
			count++;
		}
	}
	return response;
}
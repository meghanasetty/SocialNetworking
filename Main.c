#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include "Initialization.c"
#include"GroupUpdation.h"
void login();
void createfile();
int main()
{
	if (fopen(FILE_NAME, "rb+") == NULL)
		createfile();
	printf("***********************WELCOME TO  \"KNOWTHEM\"  *****************************\n");
	login();
	_getch();
}

void login()
{
	char *userID;
	int loggedout;
	userID = (char *)malloc(USERIDSIZE*sizeof(char));
	do
	{
		printf("\n\n\n\t\t\tEnter the User ID:");
		scanf(" %[^\n]s", userID);
		printf("WELCOME %s\n",userID);
		groupMenu(userID);
	} while (1);
}

void createfile()
{
	int i;
	FILE *database = fopen(FILE_NAME, "wb+");
	char *ms = "MS";//signature
	unsigned char mem[1024];//array to fill with zeroes for bit vector
	struct group groups[5];//no of groups
	int msgs[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };//mesgs direct
	int msgs_i[4] = { -1, -1, -1, -1 };//mesgs indirect
	fwrite(ms, 2, 1, database);//write signature to the file
	for (i = 0; i < 1024; i++)
	{
		mem[i] = 0;
	}
	for (i = 0; i < 1024; i++)
	{
		fwrite(mem, sizeof(mem), 1, database);//writing zeroes to file
	}
	/*creating the groups*/
	create_group(&groups[0], "user1", 0, 0, "listen to your favourite music :P", msgs, msgs_i);
	create_group(&groups[1], "user2", 0, 0, "Crazy about the sports :*", msgs, msgs_i);
	create_group(&groups[2], "user3", 0, 0, "Learn NEW things with fun", msgs, msgs_i);
	create_group(&groups[4], "user5", 0, 0, "Love about nature :D", msgs, msgs_i);
	create_group(&groups[3], "user4", 0, 0, "Imaginating things ^_^", msgs, msgs_i);
	for (i = 0; i < 5; i++)
	{
		//writing groups to the file
		long int address = FindFreeBlock(database, sizeof(struct group));
		fseek(database, address, SEEK_SET);
		fwrite(&groups[i], sizeof(groups[i]), 1, database);
	}
	fflush(database);
	fclose(database);
}


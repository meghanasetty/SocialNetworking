#include<stdio.h>
#include "MessageUpdation.h"
#include<string.h>
void groupdata(int groupno,char *);
void display_group_info(struct group groups)
{
	printf("\n\nUser Name:::%s\n", (groups.user));
	printf("users link:::%d\n", (groups.users));
	printf("no of messages:::%d\n", (groups.no_msgs));
	printf("Description:::%s\n", (groups.descriptions));
	printf("group direct:");
	for (int i = 0; i < 10; i++)
		printf("%d\t", (groups.msgs[i]));
	printf("\ngroup indirect");
	for (int i = 0; i < 4; i++)
		printf("%d\t", (groups.msgs_i[i]));
}
void create_group(struct group *groupno, char *userid, int users, int no_mesgs, char *des, int *msgs, int *msgs_i)
{
	strcpy(groupno->user, userid);//user id
	groupno->users = users;
	groupno->no_msgs = no_mesgs;
	strcpy(groupno->descriptions, des);
	for (int i = 0; i < 10; i++)
		groupno->msgs[i] = msgs[i];
	for (int i = 0; i < 5; i++)
		groupno->msgs_i[i] = msgs_i[i];
}
void groupMenu(char *userID)
{
	int groupno, exitgroup;
	do
	{
		printf("\n\n\t\t\t1.Music\n");
		printf("\t\t\t2.Sports\n");
		printf("\t\t\t3.Education\n");
		printf("\t\t\t4.Films\n");
		printf("\t\t\t5.Science Alert\n");
		printf("\t\t\t6.Log out\n");
		printf("\tEnter the group no::");
		scanf("%d", &groupno);
		if (groupno != 6)
			groupdata(groupno,userID);
	} while (groupno != 6);
}

void groupdata(int groupno,char *userID)
{
	FILE *fp = NULL;
	struct group groupinfo;
	int exit, option;
	fp = fopen(FILE_NAME, "rb+");
	int address = getGroup(groupno, &groupinfo, fp);
	do
	{
		
		printf("\n\n\n\t\t\t\t************\n\t\t\t***** Created By::::%s*****\n",groupinfo.user );
		printf("\tNO of Users::%d\n",groupinfo.users);
		printf("\tDESCRIPTION\t:::%s\n",groupinfo.descriptions);
		printf("\tNo of Posts:%d\n\n\n",groupinfo.no_msgs);
		printf("\t*****************************************************\n\t\t\t1.View Posts\n");
		printf("\t\t\t2.Add Post\n");
		printf("\t\t\t4.Exit the group");
		printf("Enter the option:");
		scanf("%d", &option);
		switch (option)
		{
		case 1:viewPosts(&groupinfo,fp,userID); break;
		case 2:addPost(&groupinfo,userID,fp); break;
		}
		fseek(fp,address,SEEK_SET);
		fwrite(&groupinfo, sizeof(struct group), 1, fp);
		
		fflush(fp);
	} while (option != 3);
	fseek(fp, address, SEEK_SET);
	(groupinfo).users += 1;
	fwrite(&groupinfo, sizeof(struct group), 1, fp);
	fflush(fp);
	fclose(fp);
}
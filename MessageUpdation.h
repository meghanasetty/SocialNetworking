#include<string.h>
#include "CommentUpdation.h"
void fill_message(struct message *mesg, char *des, char *userID);
int direct_message(struct message *mesg, int groupno);
void LikeMesg(int *msgaddress, FILE *fp, char *userID)
{
	struct message mesg;
	struct users_array users;
	int count;
	printf("Enter the count of the message:");
	scanf("%d", &count);
	fseek(fp, msgaddress[count-1], SEEK_SET);
	fread(&mesg, sizeof(struct message), 1, fp);
	mesg.likes++;
	/*fseek(fp, mesg.likes, SEEK_SET);
	fread(&users, sizeof(struct users_array), 1, fp);
	if (check_users(userID, users.user_names, users.users))
	{
		strcpy(users.user_names[users.users], userID);
		users.users++;
		
	}
	fseek(fp, mesg.likes, SEEK_SET);
	fwrite(&users,sizeof(struct users_array),1,fp);
	*/fseek(fp, msgaddress[count-1], SEEK_SET);
	fwrite(&mesg, sizeof(struct message), 1, fp);
}
int nextMesgs(int *address,FILE *fp,char *userID)
{
	int option;
	printf("\n1.Add a Comment\n");
	printf("2.Like a mesg\n");
	printf("3.Show Comments\n");
	printf("4.Next Page\n");
	printf("Enter the option:");
	scanf("%d",&option);
	switch (option)
	{
	case 1:addComment(address,fp,userID); break;
	case 2:LikeMesg(address,fp,userID); break;
	case 3:showComments(address,fp,userID); break;
	//case 4:deleteComment(address, fp, userID); break;
	case 4:return 0;
	}
	return 1;
}

void display_message(int address,FILE *fp,int count)
{
	struct message msg;
	fseek(fp,address,SEEK_SET);
	fread(&msg,sizeof(struct message),1,fp);
	printf("\n%d %s\n",count+1,msg.msg);
	printf("User: %s     ",msg.user);
	printf("likes: %d  ", msg.likes);
	printf("Comments: %d",msg.no_comments);
}
void viewPosts(struct group *groupinfo, FILE *fp,char *userID)
{
	int result, address[10];
	if ((*groupinfo).no_msgs == 0)
		printf("\nNo Posts\n");
	else
	{
		int count = (*groupinfo).no_msgs;
		int display = 0;
		int direct=0, indirect=0, indirect_i=0,i=0,j=0;
		int count_1 = 0;
		struct pointers ptr;
		while (count != 0)
		{
			for (direct; direct < 10 && count != 0; direct++)
			{
				if ((*groupinfo).msgs[direct] != -1)
				{
					address[count_1] = (*groupinfo).msgs[direct];
					//printf("\n%d   %d\n",count_1,address[count_1]);
					display_message((*groupinfo).msgs[direct],fp,count_1);
					count_1++;
					count--;
					display++;
					if (display == 5)
					{
						direct++;
						count_1=0;
						break;
					}
				}
			}
			for (indirect; indirect < 5 && count != 0&&display!=5; indirect++,j=0)
			{
				if ((*groupinfo).msgs_i[indirect] != -1)
				{
					getPointers(&ptr, (*groupinfo).msgs_i[indirect], fp);
					for (j; j < POINTERS &&count!=0; j++)
					{
						if (ptr.ptr[j] != -1)
						{
							address[count_1] = ptr.ptr[j];
							display_message(ptr.ptr[j], fp, count_1);
							count_1++;
							count--;
							display++;
							if (display == 5)
							{
								indirect++;
								count_1 = 0;
								break;
							}
						}

					}
				}
			
			}
			if (display <= 5)
			{
				result = nextMesgs(address,fp,userID);
				if (result == 1)
					break;
			}
			display = 0;
		}
		printf("no more left\n");
	}
	

}


void addPost(struct group *groupinfo,char *userID,FILE *fp)
{
	int mesgs = (*groupinfo).no_msgs;
	char mesgdes[178];
	struct message *msg = (struct message*)malloc(sizeof(struct message));
	printf("\nEnter the mesg:");
	scanf(" %[^\n]s",mesgdes);
	fill_message(msg, mesgdes,userID,fp);
	//printf("%d",sizeof(struct message));
	if (mesgs < 10)
	{
		for (int i = 0; i < 10; i++)
		if ((*groupinfo).msgs[i] == -1)
		{
			(*groupinfo).msgs[i] = direct_message(msg, fp);
			break;
		}
	}
	else
	{
		struct pointers ptr;
		int i, j;
		for (i = 0; i < 5; i++)
		{
			if ((*groupinfo).msgs_i[i] == -1)
				(*groupinfo).msgs_i[i] = createPointers(fp);
			fseek(fp, (*groupinfo).msgs_i[i], SEEK_SET);
			fread(&ptr, sizeof(struct pointers), 1, fp);
			for (j = 0; j < POINTERS; j++)
			{
				if (ptr.ptr[j] == -1)
				{
					ptr.ptr[j] = direct_message(msg, fp);
					break;
				}
			}
			fseek(fp, (*groupinfo).msgs_i[i], SEEK_SET);
			fwrite(&ptr, sizeof(struct pointers), 1, fp);
			if (j != POINTERS)
				break;
		}
		if (i == 5)
			printf("\tNO more posts allowed\n\tDELETE SOME\n");

	}
	(*groupinfo).no_msgs++;
}
int indirect_message(struct pointers ptrs, FILE *fp)
{
	int ptr_address = FindFreeBlock(fp, 128);
	fseek(fp, ptr_address, SEEK_SET);
	//printf("\n%d\n", ptr_address);
	fwrite(&ptrs, sizeof(struct pointers), 1, fp);
	return ptr_address;
}
int direct_message(struct message *mesg,FILE *fp)
{
	long int address = FindFreeBlock(fp, sizeof(struct message));
	fseek(fp, address, SEEK_SET);
	fwrite(mesg, sizeof(struct message), 1, fp);
	fflush(fp);
	return address;
}

void fill_message(struct message *mesg,char *des,char *userID,FILE *fp)
{
	strcpy(mesg->user, userID);
	mesg->no_comments = 0;
	/*struct users_array users;
	users.users = 0;
	int address = FindFreeBlock(fp, sizeof(struct users_array)); 
		printf("%d",sizeof(struct users_array));
*/		mesg->likes = 0;
		/*mesg->likes = address;
	fseek(fp,address,SEEK_SET);
	fwrite(&users,sizeof(struct users_array),1,fp);
	*/strcpy(mesg->msg, des);
	for (int i = 0; i < 10; i++)
		mesg->comments[i] = -1;
	for (int i = 0; i < 5; i++)
		mesg->comments_i[i] = -1;
}
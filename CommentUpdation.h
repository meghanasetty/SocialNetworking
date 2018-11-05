#include "structure.h"
#include<string.h>
void fill_comment(struct comment *com, char *userID, FILE *fp);
void LikeComment(int *comntaddress, FILE *fp, char *user)
{
	struct comment com;
/*	struct users_array users;
	users.users = 0;
*/	int count;
	printf("Enter the count of the comment:");
	scanf("%d",&count);
	fseek(fp, comntaddress[count - 1], SEEK_SET);
	fread(&com,sizeof(struct comment),1,fp);
/*	fseek(fp, com.likes, SEEK_SET);
	fread(&users,sizeof(struct users_array),1,fp);
	if (check_users(user, users.user_names, users.users))
	{
		users.users++;
		strcpy(users.user_names[users.users], user);
	}
	fseek(fp, com.likes, SEEK_SET);
	fwrite(&users, sizeof(struct users_array), 1, fp);
*/
	com.likes++;
	fseek(fp, comntaddress[count - 1], SEEK_SET);
	fwrite(&com,sizeof(struct comment),1,fp);
}
void display_comment(int address, FILE *fp, int count)
{
	struct comment cmt;
	fseek(fp,address,SEEK_SET);
	fread(&cmt, sizeof(struct comment), 1, fp);
	printf("\n%d %s\n",count,cmt.msg);
	printf("user:%s    ",cmt.user);
	printf("likes: %d", cmt.likes);//getUsers(cmt.likes,fp));
}
void showComments(int *msgaddress,FILE *fp,char *userId)
{
	struct message msg;
	int count;
	printf("Enter the count of the message:");
	scanf("%d",&count);
	fseek(fp,msgaddress[count-1],SEEK_SET);
	fread(&msg,sizeof(struct message),1,fp);
	int address[10];
	if ((msg).no_comments == 0)
		printf("\NO comments\n");
	else
	{
		int count = (msg).no_comments;
		int display = 0;
		int direct = 0, indirect = 0, indirect_i = 0, i = 0, j = 0;
		int count_1 = 0;
		struct pointers ptr;
		while (count != 0)
		{
			for (direct; direct < 10 && count != 0;direct++)
			{
				if ((msg).comments[direct] != -1)
				{
					address[count_1] = (msg).comments[direct];
					display_comment((msg).comments[direct], fp, count_1);
					count_1++;
					count--;
					display++;
					if (display == 5)
					{
						direct++;
						count_1 = 0;
						break;
					}
				}
			}
			for (indirect; indirect < 5 && count != 0 && display != 5; indirect++, j = 0)
			{
				if ((msg).comments_i[indirect] != -1)
				{
					getPointers(&ptr, msg.comments_i[indirect], fp);
					for (j; j < POINTERS &&count != 0; j++)
					{
						if (ptr.ptr[j] != -1)
						{
							address[count_1] = ptr.ptr[j];
							display_comment(ptr.ptr[j], fp, count_1);
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
				if (selectCommentOption(address, fp, userId))
					break;
			}
			display = 0;
		}
		printf("\nNo More left");
	}
	fseek(fp,msgaddress[count-1],SEEK_SET);
	fwrite(&msg,sizeof(struct message),1,fp);
	fflush(fp);
}

int selectCommentOption(int *address,FILE *fp,char *userId)
{
	int option;
	printf("\n1.Like comment \n");
	printf("2.Next page\n");
	printf("Enter an option:");
	scanf("%d", &option);
	if (option == 1)
	{
		LikeComment(address, fp, userId);
		return 1;
	}
	return 0;
}
void addComment(int *address, FILE *fp, char *userID)
{
	struct message msg;
	int count;
	printf("Enter the count of the message:");
	scanf("%d", &count);
	fseek(fp, address[count - 1], SEEK_SET);
	fread(&msg, sizeof(struct message), 1, fp);
	int cmnts = msg.no_comments;
	struct comment com;
	fill_comment(&com, userID, fp);
	if (cmnts < 10)
	{
		for (int i = 0; i < 10; i++)
		{
			if (msg.comments[i] == -1)
			{
				msg.comments[i] = direct_comment(com, fp);
				break;
			}
		}
	}
	else
	{
		struct pointers ptr;
		int i, j;
		for (i = 0; i < 5; i++)
		{
			if ((msg).comments_i[i] == -1)
				(msg).comments_i[i] = createPointers(fp);
			fseek(fp, (msg).comments_i[i], SEEK_SET);
			fread(&ptr, sizeof(struct pointers), 1, fp);
			for (j = 0; j < POINTERS; j++)
			{
				if (ptr.ptr[j] == -1)
				{
					ptr.ptr[j] = direct_comment(com, fp);
					break;
				}
			}
			fseek(fp, (msg).comments_i[i], SEEK_SET);
			fwrite(&ptr, sizeof(struct pointers), 1, fp);
			if (j != POINTERS)
				break;
		}
		if (i == 5)
			printf("\tNO more posts allowed\n\tDELETE SOME\n");
	}
	(msg).no_comments++;
	fseek(fp, address[count - 1], SEEK_SET);
	fwrite(&msg, sizeof(struct message), 1, fp);

}
int direct_comment(struct comment com, FILE *fp)
{
	long int address = FindFreeBlock(fp, sizeof(struct comment));
	fseek(fp, address, SEEK_SET);
	fwrite(&com, sizeof(struct comment), 1, fp);
	return address;
}
void fill_comment(struct comment *com,char *userID,FILE *fp)
{
	printf("Enter the comment:");
	scanf(" %[^\n]s",(*com).msg);
	strcpy((*com).user,userID);
	com->likes = 0;
}
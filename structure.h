#define _CRT_SECURE_NO_WARNINGS
#define SIGNATURE_SIZE 2
#define USERIDSIZE 10
#define COMMENT_SIZE 114
#define MESSAGE_SIZE 178
#define DIRECT_SIZE 10
#define INDIRECT_SIZE 5
#define DESCRIPTION_SIZE 50
#define POINTERS 32
#define POSITION_NEXT_BIT_VECTOR (1024*1024)+3
#define BLOCK_SIZE 128
#define FILE_NAME "./groupdata.bin"
#include<conio.h>
#include<math.h>
struct comment
{
	int likes;//pointer to the struct of users
	char user[USERIDSIZE];//username
	char msg[COMMENT_SIZE];//commentsize
};

struct message
{
	
	int likes;//pointer to the struct of users
	int no_comments;//no of comments per message
	int comments[DIRECT_SIZE];//direct
	int comments_i[INDIRECT_SIZE];//indirect
	char user[USERIDSIZE];
	char msg[MESSAGE_SIZE];
};

struct group
{
	int users;//pointer to the struct of users
	int no_msgs;//no of messsages per group
	int msgs[DIRECT_SIZE];
	int msgs_i[INDIRECT_SIZE];
	char user[USERIDSIZE];//user who created the group
	char descriptions[DESCRIPTION_SIZE];//description about the group

};

struct pointers
{
	int ptr[POINTERS];//seeks values for the indirect
};

struct users_array//maintaining the user info
{
	int users;
	char user_names[28][9];

};

int createPointers(FILE *fp)
{
	struct pointers ptr;
	for (int i = 0; i < POINTERS; i++)
		ptr.ptr[i] = -1;
	int address = FindFreeBlock(fp, sizeof(struct pointers));
	fseek(fp,address,SEEK_SET);
	fwrite(&ptr,sizeof(struct pointers),1,fp);
	return address;
}
void getPointers(struct pointers *ptr, int address,FILE *fp)
{
	fseek(fp, address, SEEK_SET);
	fread(ptr, sizeof(struct pointers), 1, fp);
}
/*int getUsers(int address, FILE *fp)
{
	struct users_array likes;
	fseek(fp, address, SEEK_SET);
	printf("%d",address);
	fread(&likes, sizeof(struct users_array), 1, fp);
	return likes.users;
}

int check_users(char *username, char usernames[][9], int users)
{
	for (int i = 0; i < users; i++)
	{

		if (strcmp(username, usernames[i]) == 0)
			return 0;

	}
	return 1;
}
*/
long int FindFreeBlock(FILE *fp,int size)
{
	//dividing a bit vector into 128 bytes of blocks
	unsigned char buff[128];
	int freeblck=-1;
	int startaddress,j;
	unsigned char mask_check_free = ~0;//to find the position of the free block
	long int bit_vector_blocks = 8192/*blocks in 1mb 2**20/128*/, i;
	fseek(fp, 3, SEEK_SET);
	for (i = 0; i < bit_vector_blocks; i++)
	{
		fread(buff, sizeof(buff), 1, fp);
		for (j = 0; j < 128; j++)
		{
			freeblck=check(mask_check_free, buff[j],size);//return -1 if not vacant else position of the vacent block
			if (freeblck != -1)//empty position found
				break;
		}
		if (freeblck != -1)
			break;
	}
	unsigned char mask_update;
	if (size == BLOCK_SIZE)
	{
		mask_update = 1;
		mask_update = mask_update<<freeblck;//updsting the bit vector
	}
	else
	{
		mask_update = 3;
		mask_update = mask_update<<freeblck;
	}
	buff[j] = buff[j] | mask_update;
	int offset = ((i+1)*BLOCK_SIZE);
	fseek(fp,-offset,SEEK_CUR);
	fseek(fp, j, SEEK_CUR);
	fwrite(&buff[j],sizeof(buff[j]),1,fp);//writing the filled blocks numbers 
	startaddress = (1024*1024)+((freeblck)+(j*8)+(BLOCK_SIZE*i))*BLOCK_SIZE+SIGNATURE_SIZE+1;//getting the starting address
	printf("start adress :%d\n",startaddress);
	return startaddress;
}

int check(int mask, int num,int size)
{
	int i;
	int result = mask&num;
	int bit1, bit2;
	if (result == 255)
		return -1;
	else
	{
		if (size != BLOCK_SIZE * 2)//if 2 blocks are needed
		{
			for (i = 0; i < 8; i++)
			{
				bit1 = result % 2;
				if (bit1 == 0)//break if free block is found
					break;
				result = result >> 1;
			}
		}
		else
		{
			for (i = 0; i < 8; i++)
			{
				bit1 = result % 2;
				result = result >> 1;
				bit2 = result % 2;
				if (bit1 == 0 && bit2 == 0)//if two blocks found
					break;
			}
			if (i == 7)//if last bit is free then we have only one bit
				return -1;

			
		}
		return i;
	}
}

int getGroup(int groupno, struct group *grp, FILE *fp)
{
	int address = (1024 * 1024) + SIGNATURE_SIZE + 1 + (BLOCK_SIZE*(groupno - 1));//address of the group
	fseek(fp, address, SEEK_SET);//seeking there
	fread(grp, sizeof(struct group), 1, fp);//reading to the structure
	return address;
}
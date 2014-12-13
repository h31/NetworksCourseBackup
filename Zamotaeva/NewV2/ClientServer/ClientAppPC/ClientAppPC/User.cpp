#include "StdAfx.h"
#include "User.h"
#include <stdlib.h>
#include "MyCreator.h"

UserList ParseUsers(char * data)
{
	UserList userlist;
	int shift = 0;
	//get user count
	char * c_user_count = getCleanBuffer(USERS_COUNT_FIELD_SIZE);
	int user_count = 0;
	for(int i = 0; i < USERS_COUNT_FIELD_SIZE;i++ )
	{
		c_user_count[i] = data[shift];
		shift++;
	}
	sscanf(c_user_count,"%d",&user_count);
	//User* users =  (User*)malloc(sizeof(User)*user_count);
	User* users = new User[user_count];

	//get all users (name and name size)
	char* c_name_len = getCleanBuffer(NAME_LEN_FIELD_SIZE+1);
	int name_len = 0;
	char* name;
	for(int n = 0; n< user_count; n++)
	{
		//get name length
		for(int i = 0;i<NAME_LEN_FIELD_SIZE;i++)
		{
			c_name_len[i] = data[shift];
			shift++;
		}
		sscanf(c_name_len,"%d",&name_len);

		//get name
		//@dev: check name_len < 0 condition
		name = getCleanBuffer(name_len+1);//(char*)malloc(sizeof(char)*name_len);
		for(int j = 0; j<name_len; j++)
		{
			 name[j] = data[shift];
			 shift++;
		}
		name[name_len] = '\0';

		//set user fields
		User user;
		user.name = name;
		user.name_len = name_len;
		users[n] = user;
	}

	userlist.list_len = user_count;
	userlist.users  = users;
	return userlist;
}

UserList initEmptyList()
{
	UserList emptylist;
	emptylist.list_len = 0;
	emptylist.users = NULL;
	return emptylist;
}

User EmptyUser()
{
	User empty;
	empty.name_len = -1;
	return empty;
}

bool isEmptyUser(User user)
{
	return (user.name_len == -1);
}
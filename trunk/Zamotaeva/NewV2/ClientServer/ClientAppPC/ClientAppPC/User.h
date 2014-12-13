#ifndef _USER_H
#define _USER_H


const int USERS_COUNT_FIELD_SIZE = 3;
const int NAME_LEN_FIELD_SIZE = 2;

struct User
{
	int name_len;
	char * name;
};


struct UserList
{
	int list_len;
	User * users; //array of users
};


UserList ParseUsers(char * data);
UserList initEmptyList();

User EmptyUser();
bool isEmptyUser(User user);

#endif

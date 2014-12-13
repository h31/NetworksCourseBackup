#include "stdafx.h"
#include "UserList.h"
#include "string.h"
#include "StringsHelper.h"
#include "MyLogger.h"
#include "MyServer.h"

UserList::UserList(void)
{
}


UserList::~UserList(void)
{
}


void UserList::DecreaseTimeUsers(int decValue_ms) //decrement time for all users
{
	for(unsigned int i=0;i<this->users.size();i++)
	{
		users[i].DecreaseLiveTime(decValue_ms);
	}
}


mysocket UserList::getSocketByName(string name)
{
	for(unsigned int i=0;i<this->users.size();i++)
	{
		User user = users[i];
		if(name == user.getUserName())
		{
			return user.getUserSocket();
		}
	}
	return INVALID_SOCKET;
}

sockaddr_in  UserList::getAddrByName(string name)
{
	sockaddr_in addr;
	for(unsigned int i=0;i<this->users.size();i++)
	{
		User user = users[i];
		if(name == user.getUserName())
		{
			return user.getUserAddr();
		}
	}
	return addr;
}

bool UserList::isNameUnique(string name)
{
	for(unsigned int i=0;i<this->users.size();i++)
	{
		User user = users[i];
		if (name == user.getUserName())
		{
			return false;
		}

	}
	return true;

}

bool UserList::isNameRight(string name)
{
	for(int i = 0; i< name.size();i++)
	{
		int i_symb  = (int)(name[i]);
		if(i_symb < 0 || i_symb > 255)
		{
			return false;
		}
	}
	if(name.length() <= 0)
	{
		return false;
	}
	if(name.length() > 32)
	{
		return false;
	}
	return true;
}

User* UserList::getUserBySocketDesc(mysocket s)
{
	for(unsigned int i=0;i<this->users.size();i++)
	{
		if(users[i].getUserSocket() == s)
		{
			return &users[i];
		}

	}
	return NULL;
}

bool UserList::RestoreTimeBySocket(mysocket s)
{
	User* user = getUserBySocketDesc(s);
	if(user != NULL)
	{
		user->RestoreLiveTime();
		return true;
	}
	return false;

}


bool UserList::tryAddNewUser(User* user)
{
	this->users.push_back(*user);
	return true;
}
bool UserList::tryAddNewUser(string name,mysocket s)
{
	User *user = new User(name,s);
	tryAddNewUser(user);
	return true;
}

string UserList::getUserList()
{

	string userlist = "";


	char * c_users_count = StringsHelper::getCleanBuffer(USERS_COUNT_FIELD_SIZE);
	int users_count = users.size();
	sprintf(c_users_count,"%3d",users_count);
	userlist +=c_users_count;

	for(int i=0;i<userlist.size();i++)
	{
		if(userlist[i] == ' ')
		{
			userlist[i] = '0';
		}
	}
	char * c_name_len = StringsHelper::getCleanBuffer(NAME_LEN_FIELD_SIZE);

	for(int i=0;i<users.size();i++)
	{
		int name_len = users[i].getUserName().size();
		sprintf(c_name_len,"%2d",name_len);
		c_name_len = StringsHelper::replaceEmptySymbols(c_name_len,2);
		userlist+=c_name_len;
		userlist+=users[i].getUserName();
	}
	//userlist+= '\0';
	return userlist;
}

bool UserList::isAuthorizedUser(mysocket s)
{
	for(unsigned int i=0;i<this->users.size();i++)
	{
		if(users[i].getUserSocket() == s)
		{
			return true;
		}
	}
	return false;
}

bool UserList::isAuthorizedUser(sockaddr_in addr)
{
	/*
	for(unsigned int i=0;i<this->users.size();i++)
	{
		if(users[i].getUserAddr() == addr)
		{
			return true;
		}
	}
	*/
	return true;
}

bool UserList::deleteTimeOutedUsers()
{
	bool delAtLeastOneUser = false;
	for(int i = users.size() - 1; i >=0; i--)
	{
		if(users[i].getLiveTime() <= 0) 
		{
			MyLogger::WriteObjectInfo("user deleted[timeout]",(char*)(users[i].getUserName().c_str()));
			mysocket s = users[i].getUserSocket();

			MyServer::closeSocket(s);
			users.erase(users.begin() + i);
			delAtLeastOneUser = true;

		}
	}
	return delAtLeastOneUser;
}


bool UserList::deleteQuitUser(mysocket s)
{
	bool delAtLeastOneUser = false;
	for(int i = users.size() - 1; i >=0; i--)
	{
		if(users[i].getUserSocket() == s)
		{
			MyLogger::WriteObjectInfo("user deleted[quit]",(char*)(users[i].getUserName().c_str()));
			MyServer::closeSocket(s);
			users.erase(users.begin() + i);
			delAtLeastOneUser = true;
		}
	}
	return delAtLeastOneUser;
}

bool UserList::deleteQuitUser(sockaddr_in addr)
{
	bool delAtLeastOneUser = false;
	for(int i = users.size() - 1; i >=0; i--)
	{
		/*
		if(users[i].getUserAddr() == addr)
		{
			MyLogger::WriteObjectInfo("user deleted[quit]",(char*)(users[i].getUserName().c_str()));
			users.erase(users.begin() + i);
			delAtLeastOneUser = true;
		}
		*/
	}
	return delAtLeastOneUser;
}
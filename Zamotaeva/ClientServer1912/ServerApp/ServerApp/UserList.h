#pragma once
#include <vector>
#include "User.h"
//#include "MyServer.h"
class UserList
{
public:
	static const int USERS_COUNT_FIELD_SIZE = 3;
	static const int NAME_LEN_FIELD_SIZE = 2;
public:
	std::vector<User> users;
public:
	UserList();
	~UserList();
    mysocket getSocketByName(string name);
	sockaddr_in getAddrByName(string name);
    User* getUserBySocketDesc(mysocket s);
    bool RestoreTimeBySocket(mysocket s);
    bool isNameUnique(string name);
    bool isNameRight(string name);
	bool tryAddNewUser(User * user);
	bool tryAddNewUser(string name,mysocket s);
    void DecreaseTimeUsers(int decValue_ms);
	bool isAuthorizedUser(mysocket s);
	bool isAuthorizedUser(sockaddr_in addr);

	bool deleteTimeOutedUsers();
	bool deleteQuitUser(mysocket s);
	bool deleteQuitUser(sockaddr_in addr);
	string getUserList();

	bool CompareAddr(sockaddr_in addr1, sockaddr_in addr2);
};


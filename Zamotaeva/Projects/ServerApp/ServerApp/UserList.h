#pragma once
#include <vector>
#include "User.h"
class UserList
{
private:
	std::vector<User> users;
public:
	UserList();
	~UserList();
    string getIPByName(string name);
    User* getUserByIP(string ip);
    bool RestoreTimeByIP(string ip);
    bool isNameUnique(string name);
    bool isNameRight(string name);
	bool tryAddNewUser(User user);
	bool tryAddNewUser(char * name, char * ip);
    void DecreaseTimeUsers(int decValue_ms);
};


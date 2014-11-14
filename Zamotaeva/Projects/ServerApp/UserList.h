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
    char * getIPByName(char* name);
    bool checkNameUnique(char *name); //unik name v seti
	bool tryAddNewUser(User user);
	bool tryAddNewUser(char * name, char * ip);
    void DecreaseTimeUsers(int decValue_ms);
};


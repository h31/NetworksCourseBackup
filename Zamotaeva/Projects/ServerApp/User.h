#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;


class User
{
public:
	User(void);
	~User(void);
private:
	string userName;
	unsigned int userID;
	string curIP;
};


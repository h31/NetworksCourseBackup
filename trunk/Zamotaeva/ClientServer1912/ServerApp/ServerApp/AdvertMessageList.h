#pragma once

#include <vector>
#include <string>
using namespace std;

class AdvertMessageList
{
private:
	vector<string>advertList;
	int Counter;
public:
	AdvertMessageList(void);
	~AdvertMessageList(void);
	string getNextAdvert();
};


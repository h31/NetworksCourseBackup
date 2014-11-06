#pragma once

#include <winsock.h>
class GetMyIP
{
public:
	GetMyIP(void);
	~GetMyIP(void);
	static char* getIPAddress();
};


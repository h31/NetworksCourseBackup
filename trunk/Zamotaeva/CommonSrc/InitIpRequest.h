#pragma once
#include "Command.h"
class InitIpRequest : public Command
{
public:
	InitIpRequest(void);
	~InitIpRequest(void);
	BYTE* GenerateData();
};


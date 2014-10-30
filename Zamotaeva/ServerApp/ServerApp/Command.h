#pragma once
#include "CommandsIDs.h"
class Command
{
public:
	int commandID; //ident
	Command(int commandID);
	~Command(void);
	BYTE * data;// data
};


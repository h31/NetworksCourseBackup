#pragma once
#include "CommandsIDs.h"

class Command
{
public:
	int commandID; //ident
	Command(int commandID);
	~Command(void);
    char * data;// data
	int data_len;
	void OutCommand();
};


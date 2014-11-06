#pragma once
#include "CommandsIDs.h"

typedef unsigned char BYTE;
class Command
{
public:
	int commandID; //ident
	Command(int commandID);
	~Command(void);
	BYTE * data;// data
	int data_len;
	void OutCommand();
};


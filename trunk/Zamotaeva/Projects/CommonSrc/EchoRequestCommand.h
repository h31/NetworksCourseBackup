#pragma once
#include "Command.h"
class EchoRequestCommand :
	public Command
{
public:
	EchoRequestCommand(void);
	~EchoRequestCommand(void);
    char* GenerateData();
};


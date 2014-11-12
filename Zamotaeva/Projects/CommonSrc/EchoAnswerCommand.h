#pragma once
#include "Command.h"
class EchoAnswerCommand :
	public Command
{
public:
	EchoAnswerCommand(void);
	~EchoAnswerCommand(void);
    char* GenerateData();
};


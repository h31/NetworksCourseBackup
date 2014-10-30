#pragma once
#include "command.h"
class EchoAnswerCommand :
	public Command
{
public:
	EchoAnswerCommand(void);
	~EchoAnswerCommand(void);
	BYTE* GenerateData();
};


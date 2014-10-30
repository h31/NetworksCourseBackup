#pragma once
#include "c:\users\tatianka\documents\visual studio 2010\projects\clientserver\serverapp\serverapp\command.h"
class EchoRequestCommand :
	public Command
{
public:
	EchoRequestCommand(void);
	~EchoRequestCommand(void);
	BYTE* GenerateData();
};


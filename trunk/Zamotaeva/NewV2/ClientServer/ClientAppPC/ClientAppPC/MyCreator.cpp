#include "stdafx.h"
#include "MyCreator.h"
#include "MyClient.h"
#include <stdlib.h>


char* getCleanBuffer(int Size)
{
	if(Size <= 0)
	{
		Size = MAX_PACKAGE_SIZE;	//default buffer size
	}
	char * buffer;
	buffer = (char*) malloc (sizeof(char)* Size);
	//buffer = new char[Size];
	for(int i =0; i< Size; i++)
	{
		buffer[0] = '\0';
	}
	return buffer;
}

char checkSymbol(char symb)
{
	if((int)symb < 0 || symb > (255) )
	{
		return ' ';
	}
	if(symb >'a' && symb <'z')
	{
		return symb;
	}
	if(symb > 'A' && symb < 'Z')
	{
		return symb;
	}
	/*
	if(symb  > 'À' && symb < 'ß')
	{
		return symb;
	}
	if(symb >'à' && symb < 'ÿ')
	{
		return symb;
	}
	*/
	if(symb == ' ' || symb == '_' || symb == '-' || symb =='.' || symb == '!' || symb == '?'|| symb == '+' || symb == '/')
	{
		return symb;
	}
	if(symb == ',' || symb == ':')
	{
		return symb;
	}

	return ' ';
}
#include "stdafx.h"
#include "StringsHelper.h"


char*  StringsHelper::getCleanBuffer(int Size)
{
	char * buffer = new char[Size];
    for(int i=0;i<Size;i++)
	{
		buffer[i] = '\0';
	}
	return buffer;
}

char* StringsHelper::replaceEmptySymbols(char * sourse, int size)
{
	for(int i = 0;i<size;i++)
	{
		if(sourse[i] == ' ')
		{
			sourse[i] = '0';
		}
	}
	return sourse;
}

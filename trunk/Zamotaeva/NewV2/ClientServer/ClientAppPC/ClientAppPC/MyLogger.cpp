#include "stdafx.h"
#include "MyLogger.h"


void WriteSuccessNetworkEvent(char * text)
{
	setSuccessColor();
	printf("Success:\t%s\n",text);
	setNormalColor();
}

void WriteSuccessNetworkEvent(char * text,char *object)
{
	setSuccessColor();
	printf("Success:\t%s  - %s\n",text,object);
	setNormalColor();
}

void WriteFailNetworkEvent(char * text)
{
	setErrorColor();
	printf("%s\n",text);
	setNormalColor();
}

void WriteFailEvent(char * text)
{
	setErrorColor();
	printf("%s\n",text);
	setNormalColor();
}

void WriteMessage(MessageStructure message_str)
{
	setMessageColor();
	OutMessage(message_str);
	setNormalColor();
}

void WriteAllMessages(MessageCollection* message_collection)
{
	setMessageColor();
	for(int i = 0;i<message_collection->readed_size;i++)
	{
		OutMessage(message_collection->readed[i]);
		
	}
	setNormalColor();
	printf("new\n");
	setMessageColor();
	for(int j = 0;j<message_collection->unreaded_size;j++)
	{
		OutMessage(message_collection->unreared[j]);
	}
	setNormalColor();
	
}


void WritePackageInfo(char * text,char * status)
{
	setPackageColor();
	printf("%s package:\t%s\n",status,text);
	setNormalColor();
}

void WriteObjectInfo(char *object,char * text)
{
	setWarningColor();
	printf("%s:\t%s\n",object,text);
	setNormalColor();
}
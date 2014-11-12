#include "MessageStructure.h"
#include <stdlib.h>
#include <iostream>
#include <stdio.h>

int getMessageTextsSumm(MessageStructure message)
{
	return message.message_len + message.sender_len + message.time_len+message.dest_len;
}
void OutMessage()
{

}
MessageStructure parseData(char* data)
{
	//example
	// 1s 5gekko25Mon Nov 10 01:22:24 2014 9123456789
	int i=0;
	int shift = 0;

	int dest_len = 1;
	sscanf(data,"%2d",&dest_len);
	char * dest = (char*)malloc(sizeof(char)*dest_len);
	shift = 2;
	for( i =0; i< dest_len;i++)
	{
		dest[i] = data[shift+i];
	}
	MessageStructure message;
	return message;
}


char * ConvertMessageToData(MessageStructure message)
{

	int full_len = SERVICE_FIELDS_SIZE + getMessageTextsSumm(message);
	char * data = (char*)malloc(sizeof(char)*full_len);

	char *formatmessagedata = (char*)malloc(sizeof(char)*30);
	sprintf(formatmessagedata,"%s%d%s%d%s%d%s%d%s", "%2d%",message.dest_len,"s%2d%",message.sender_len,"s%2d%",message.time_len,"s%3d%",message.message_len,"s");
	sprintf(data,formatmessagedata,message.dest_len,message.dest_name,message.sender_len,message.sender_name,message.time_len,message.time_stamp,message.message_len,message.message);
	return data;
}

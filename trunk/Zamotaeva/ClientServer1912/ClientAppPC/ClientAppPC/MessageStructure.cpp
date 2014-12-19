
#include <stdlib.h>
#include <iostream>
#include <stdio.h>

#include "stdafx.h"
#include "MessageStructure.h"
#include "MyCreator.h"
#include <ctype.h>

int getMessageTextsSumm(MessageStructure message)
{
	return message.message_len + message.sender_len + message.time_len+message.dest_len;
}
void OutMessage(MessageStructure message)
{
	//print time stamp
	printf("[%s]",message.time_stamp);
	//print sender
	for(int i = 0;i<message.sender_len;i++)
	{
		char symb = message.sender_name[i];
		symb = checkSymbol(symb);
		printf("%c",symb);
		
	}
	printf(":");
	//print message
	for(int i = 0;i<message.message_len;i++)
	{
		int symb = (int)message.message[i];
		symb = checkSymbol(symb);
		printf("%c",symb);
	}
	printf("\n");
	//printf("[%s] %s:%s\n",message.time_stamp,message.sender_name,message.message); //15:01 jul hi
}
MessageStructure parseData(char* data) //receive data from socket and parse
{
	//example  - field data
	//05julia05gekko25Mon Nov 10 01:22:24 2014 005hello


	int shift = 0;

	//parse receiver name
	char *c_receiver_len = getCleanBuffer(DEST_FIELD_SIZE);
	int receiver_len = 0;
	memcpy(c_receiver_len,&data[shift],DEST_FIELD_SIZE);
	shift+=DEST_FIELD_SIZE;
	sscanf(c_receiver_len,"%2d",&receiver_len);
	char * receiver = getCleanBuffer(receiver_len+1);
	memcpy(receiver, &data[shift],receiver_len);
	receiver[receiver_len] = '\0';
	shift+=receiver_len;


	//parse sender name
	char *c_sender_len = getCleanBuffer(SENDER_NAME_FIELD_SIZE);
	int sender_len = 0;
	memcpy(c_sender_len,&data[shift],SENDER_NAME_FIELD_SIZE);
	shift+=SENDER_NAME_FIELD_SIZE;
	sscanf(c_sender_len,"%2d", &sender_len);
	char *sender = getCleanBuffer(sender_len+1);
	memcpy(sender,&data[shift],sender_len);
	sender[sender_len] = '\0';
	shift+=sender_len;

	//parse timestamp
	char *c_timestamp_len = getCleanBuffer(TIMESTAMP_FIELD_SIZE);
	int timestamp_len = 0;
	memcpy(c_timestamp_len,&data[shift],TIMESTAMP_FIELD_SIZE);
	shift+=TIMESTAMP_FIELD_SIZE;
	sscanf(c_timestamp_len,"%2d",&timestamp_len);
	char *timestamp = getCleanBuffer(timestamp_len+1);
	memcpy(timestamp,&data[shift],timestamp_len);
	timestamp[timestamp_len] = '\0';
	shift+=timestamp_len;

	//parse message text;
	char * c_message_len = getCleanBuffer(MESSAGE_FIELD_SIZE);
	int message_len = 0;
	memcpy(c_message_len,&data[shift],MESSAGE_FIELD_SIZE);
	shift+=MESSAGE_FIELD_SIZE;
	sscanf(c_message_len,"%3d",&message_len);
	char* message_text = getCleanBuffer(message_len+1);
	message_text[message_len] = '\0';
	memcpy(message_text,&data[shift],message_len);
	shift+=message_len;

	//completestructure
	MessageStructure message;
	message.dest_name = receiver;
	message.dest_len = receiver_len;
	message.sender_name = sender;
	message.sender_len = sender_len;
	message.message = message_text;
	message.message_len = message_len;
	message.time_stamp = timestamp;
	message.time_len = timestamp_len;
	return message;
}


char * ConvertMessageToData(MessageStructure message)
{

	int full_len = SERVICE_FIELDS_SIZE + getMessageTextsSumm(message);
	char * data = getCleanBuffer(full_len);

	char *formatmessagedata = getCleanBuffer(30);
	sprintf(formatmessagedata,"%s%d%s%d%s%d%s%d%s", "%2d%",message.dest_len,"s%2d%",message.sender_len,"s%2d%",message.time_len,"s%3d%",message.message_len,"s");
	sprintf(data,formatmessagedata,message.dest_len,message.dest_name,message.sender_len,message.sender_name,message.time_len,message.time_stamp,message.message_len,message.message);
	return data;
}


MessageStructure generateMessage(char * message_text,char * sender_name, User user, char * timestamp)
{
	MessageStructure msg;
	msg.message = message_text;
	msg.message_len = strlen(message_text);
	msg.dest_name = user.name;
	msg.dest_len = user.name_len;
	msg.sender_name = sender_name+'\0';
	msg.sender_len = strlen(sender_name);
	msg.time_stamp = timestamp;
	msg.time_len = strlen(timestamp);

	return msg;
}


MessageStructure convertAdvertToMessage(char * message_text)
{
	MessageStructure msg;
	msg.message = message_text;
	msg.message_len = strlen(message_text);
	msg.dest_name = " ";
	msg.dest_len = 1;
	msg.sender_name = "server";
	msg.sender_len = 6;
	msg.time_stamp = "now";
	msg.time_len = 3;

	return msg;
}
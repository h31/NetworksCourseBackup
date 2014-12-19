#include "stdafx.h"
#include "MyMessage.h"
#include "StringsHelper.h"


MyMessage::MyMessage(void)
{
}

MyMessage::MyMessage(string sender_name, string receiver_name, string message,string timestamp)
{
	this->message = message;
	this->sender_name = sender_name;
	this->receiver_name = receiver_name;
	this->timestamp = timestamp;
}


MyMessage::~MyMessage(void)
{
}

MyMessage MyMessage::ConvertDataToMessage(string data)
{
	MyMessage message;
	int shift = 0;
	
	//parse receiver name
	string s_dest_len = data.substr(shift,DEST_FIELD_SIZE);
	shift +=DEST_FIELD_SIZE;
	int dest_len = atoi(s_dest_len.c_str());
	string dest = data.substr(shift,dest_len);
	shift+=dest_len;

	
	//parse sender name
	string s_sender_len = data.substr(shift,SENDER_NAME_FIELD_SIZE);
	shift+=SENDER_NAME_FIELD_SIZE;
	int sender_len = atoi(s_sender_len.c_str());
	string sender = data.substr(shift,sender_len);
	shift+=sender_len;


	//parse timestamp
	string s_timestamp_len = data.substr(shift,TIMESTAMP_FIELD_SIZE);
	shift+=TIMESTAMP_FIELD_SIZE;
	int timestamp_len = atoi(s_timestamp_len.c_str());
	string timestamp = data.substr(shift,timestamp_len);
	shift+=timestamp_len;

	//parse message 
	string s_message_len = data.substr(shift,MESSAGE_FIELD_SIZE);
	shift +=MESSAGE_FIELD_SIZE;
	int message_len = atoi(s_message_len.c_str());
	string message_text = data.substr(shift,message_len);

	//add data to message structure and return structure
	message.receiver_name = dest;
	message.sender_name = sender;
	message.timestamp = timestamp;
	message.message = message_text;
	return message;

}

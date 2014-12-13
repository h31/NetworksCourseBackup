#pragma once

#include <iostream>

using namespace std;
class MyMessage
{

public:
	static const int DEST_FIELD_SIZE  = 2;
	static const int SENDER_NAME_FIELD_SIZE  = 2;
	static const int TIMESTAMP_FIELD_SIZE  = 2;
	static const int MESSAGE_FIELD_SIZE  = 3;
	static const int SERVICE_FIELDS_SIZE = DEST_FIELD_SIZE + SENDER_NAME_FIELD_SIZE + TIMESTAMP_FIELD_SIZE + MESSAGE_FIELD_SIZE; 
public:
	MyMessage(void);
	MyMessage(string sender_name, string receiver_name, string message,string timestamp);
	~MyMessage(void);
	string receiver_name;
	string sender_name;

	string message;
	string timestamp;
	
	bool correct_data;
	bool correct_receiver;

	static MyMessage ConvertDataToMessage(string);
};


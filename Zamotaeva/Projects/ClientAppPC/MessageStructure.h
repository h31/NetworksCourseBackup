#ifndef _MESSAGESTRUCTURE_H_
#define _MESSAGESTRUCTURE_H_

struct MessageStructure
{
	char * sender_name;
	char * message;
	char * time_stamp;
	char * dest_name;
	int message_len;
	int sender_len;
	int time_len;
	int dest_len;

};

static const int DEST_FIELD_SIZE  = 2;
static const int SENDER_NAME_FIELD_SIZE  = 2;
static const int TIMESTAMP_FIELD_SIZE  = 2;
static const int MESSAGE_FIELD_SIZE  = 3;
static const int SERVICE_FIELDS_SIZE = DEST_FIELD_SIZE + SENDER_NAME_FIELD_SIZE + TIMESTAMP_FIELD_SIZE + MESSAGE_FIELD_SIZE; 

char * ConvertMessageToData(MessageStructure);
int getMessageTextsSumm(MessageStructure);
void OutMessage();
MessageStructure parseData(char*);
#endif
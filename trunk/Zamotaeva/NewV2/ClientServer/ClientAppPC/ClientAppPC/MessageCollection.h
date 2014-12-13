#pragma once
#include "MessageStructure.h"

struct MessageCollection
{
	static const int INIT_READ_ALLOC_SIZE = 50;
	static const int INIT_UNREAD_ALLOC_SIZE = 20;
	int readed_size;
	int unreaded_size;
	int read_allocated_size;
	int unread_allocated_size;
	MessageStructure * readed;
	MessageStructure *unreared;
};


void AddNewUnReadedMessage(MessageCollection* message_collection, MessageStructure newMessage);
void MoveAllUnReadToRead(MessageCollection* message_collection);

MessageCollection initEmptyCollection();
MessageCollection* initEmptyCollectionPointer();
#include "stdafx.h"
#include "MessageCollection.h"

//@dev: free memory?
void AddNewUnReadedMessage(MessageCollection* message_collection, MessageStructure newMessage)
{
	int new_size = message_collection->unreaded_size + 1;
	if(new_size < message_collection->unread_allocated_size)
	{
		message_collection->unreared[new_size - 1] = newMessage;
	}
	message_collection->unreaded_size  = new_size;
}


void MoveAllUnReadToRead(MessageCollection* message_collection)
{
	//message_collection->unreared = 
}
MessageCollection initEmptyCollection()
{
	MessageCollection message_collection;
	message_collection.readed_size = 0;
	message_collection.unreaded_size = 0;
	message_collection.read_allocated_size = message_collection.INIT_READ_ALLOC_SIZE;
	message_collection.unread_allocated_size = message_collection.INIT_UNREAD_ALLOC_SIZE;
	message_collection.readed = (MessageStructure*)malloc(sizeof(MessageStructure)*message_collection.read_allocated_size);
	message_collection.unreared = (MessageStructure*)malloc(sizeof(MessageStructure)*message_collection.unread_allocated_size);
	return message_collection;
}

MessageCollection* initEmptyCollectionPointer()
{
	MessageCollection* message_collection = (MessageCollection*)malloc(sizeof(MessageCollection)*1);
    message_collection->readed_size = 0;
    message_collection->unreaded_size = 0;
    message_collection->read_allocated_size = message_collection->INIT_READ_ALLOC_SIZE;
    message_collection->unread_allocated_size = message_collection->INIT_UNREAD_ALLOC_SIZE;
    message_collection->readed = (MessageStructure*)malloc(sizeof(MessageStructure)*message_collection->read_allocated_size);
    message_collection->unreared = (MessageStructure*)malloc(sizeof(MessageStructure)*message_collection->unread_allocated_size);
	return message_collection;
}

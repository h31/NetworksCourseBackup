#pragma once
#include "Command.h"
class Package
{
public:
	static const int LENGTH_FIELD_SIZE = 3; //dlina vsego packeta
	static const int TYPE_FIELD_SIZE = 2;	//type command
	static const int SENDER_FIELD_SIZE = 15; //name polz - kto otprav
	static const int DATA_FIELD_SIZE = 4;
	static const int DATA_START_POSITION = LENGTH_FIELD_SIZE+TYPE_FIELD_SIZE+SENDER_FIELD_SIZE;//mesto data
public:
	Package(void);
	~Package(void);
    Command Parse(char* inputmessage); //rashifr
	Package Generate(Command cmd); //gener package
    char * data;
	int data_length;
	int packagesize;

	void OutPackage();
	
};


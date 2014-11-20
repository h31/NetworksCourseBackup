#pragma once
#include "Command.h"
class Package
{
public:
    static const int LENGTH_FIELD_SIZE = 3;
    static const int TYPE_FIELD_SIZE = 2;
    static const int DATA_START_POSITION = LENGTH_FIELD_SIZE+TYPE_FIELD_SIZE;
    static const int MAX_PACKAGE_SIZE = 512;
public:
	Package(void);
	~Package(void);
    static Command Parse(char* inputmessage); //convert inputmessage to command (id+data)
    Package ParseToPackage(char* inputmessage);
    static Package Generate(Command cmd); 
    char * data;
	int data_length;
	int packagesize;

	void OutPackage();
	
};


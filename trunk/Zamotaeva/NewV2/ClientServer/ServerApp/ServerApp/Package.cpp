#include "Package.h"
#include <stdio.h>
#include "../ServerApp/StringsHelper.h"
Package::Package(void)
{
}


Package::~Package(void)
{
}

Command Package::Parse(char* inputmessage)
{
	int shift = 0;
	//parse package length
	char *c_packlen = StringsHelper::getCleanBuffer(LENGTH_FIELD_SIZE);
	int i_packlen = 0;
	for(int i=0; i<Package::LENGTH_FIELD_SIZE;i++)
	{
		c_packlen[i] =inputmessage[shift];
		shift++;
	}
	sscanf(c_packlen,"%d",&i_packlen);

	inputmessage[i_packlen] = '\0';


	//parse command id
	char *c_packtype = StringsHelper::getCleanBuffer(TYPE_FIELD_SIZE);
	int i_packtype = 0;
	for(int i=0; i<Package::TYPE_FIELD_SIZE;i++)
	{
		c_packtype[i] =inputmessage[shift];
		shift++;
	}
    sscanf(c_packtype,"%d",&i_packtype); //convert type char-->int ('00' - 0)


	//get package data
	int data_size = i_packlen - shift+1;
	string templine = "";
	if(data_size > 0)
	{	
		for(int i = 0;i<data_size;i++)
		{
			templine += inputmessage[shift];
			shift++;
		}
		templine[data_size-1] = '\0';

	}
	


    //generate command - type,data
    Command command(i_packtype);
    command.data = templine;
	command.data_len = templine.size();

	//destroy pointers

	//return commands
	return command;
}

Package Package::Generate(Command cmd) //generate package from command
{
    Package pack;
    pack.data_length = cmd.data_len;
	if(cmd.data_len < 0)
	{
		return pack;
	}
    pack.packagesize = Package::DATA_START_POSITION + pack.data_length;
    char * packagedata = StringsHelper::getCleanBuffer(pack.packagesize);


    for(int i=0;i< pack.packagesize;i++)
	{
		packagedata[i] = ' ';
	}

    for(int i=0;i< pack.data_length;i++)
	{
		packagedata[DATA_START_POSITION+i] = cmd.data[i]; 
	}
	int commandID = cmd.commandID;
	char commID_char[TYPE_FIELD_SIZE+1] = {};
	sprintf(commID_char,"%2i",commandID); 

	char length_char[LENGTH_FIELD_SIZE+1] = {};
    sprintf(length_char,"%3i",pack.packagesize); 


	for(int i = 0;i < Package::LENGTH_FIELD_SIZE;i++)
	{
		packagedata[i] = length_char[i];
	}

	for(int i = 0;i < Package::TYPE_FIELD_SIZE;i++)
	{
		packagedata[i+LENGTH_FIELD_SIZE] = commID_char[i];
	}

	//replace empty char to '0'
    for(int i=0;i< pack.packagesize;i++)
	{
		if(packagedata[i]==' ')
		{
			packagedata[i] = '0';
		}
	}
    pack.data = packagedata;
	return pack;
}


void Package::OutPackage()
{
	printf("[");
	int shift = 0;

	for(int i = 0; i< LENGTH_FIELD_SIZE;i++)
	{
		printf("%c",this->data[shift]);
		shift++;
	}
	printf("]");

	printf("[");
	for(int i = 0; i< TYPE_FIELD_SIZE;i++)
	{
		printf("%c",this->data[shift]);
		shift++;
	}
	printf("]");

	printf("[");
	for(int i = 0; i< this->data_length;i++)
	{
		printf("%c",this->data[shift]);
		shift++;
	}
	printf("]");

	printf("\n");
}

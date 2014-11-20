#include "Package.h"
#include <stdio.h>
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
	char *c_packlen = new char[LENGTH_FIELD_SIZE];
	int i_packlen = 0;
	for(int i=0; i<Package::LENGTH_FIELD_SIZE;i++)
	{
		c_packlen[i] =inputmessage[shift];
		shift++;
	}
	sscanf(c_packlen,"%d",&i_packlen);
 
	//int real_length =  (sizeof(*inputmessage)/sizeof(BYTE));
	/*
	if(real_length != i_packlen)
	{
		//return bad package
		int ID = CommandsIDs::BAD_PACKAGE_ANSWER;
		Command  cmd(ID);
		return cmd;
	}
	*/

	//parse package type
	char *c_packtype = new char[TYPE_FIELD_SIZE];
	int i_packtype = 0;
	for(int i=0; i<Package::TYPE_FIELD_SIZE;i++)
	{
		c_packtype[i] =inputmessage[shift];
		shift++;
	}
    sscanf(c_packtype,"%d",&i_packtype); //convert type char-->int ('00' - 0)


	//get package data
	int data_size = i_packlen - shift+1;
	char *c_packdata;
	if(data_size > 0)
	{
		c_packdata = new char[data_size];
	}
	for(int i = 0;i<data_size;i++)
	{
		c_packdata[i] = inputmessage[shift];
		shift++;
	}


    //generate command - type,data
    Command command(i_packtype);
    command.data = c_packdata;

	//destroy pointers

	//return commands
	return command;
}

Package Package::Generate(Command cmd) //generate package from command
{
    Package pack;
    pack.data_length = cmd.data_len;
    pack.packagesize = Package::DATA_START_POSITION + pack.data_length; //dl package = slujebnoy inf + data
    char * packagedata = new char[pack.packagesize]; //ves package


    for(int i=0;i< pack.packagesize;i++)
	{
		packagedata[i] = ' ';
	}

    for(int i=0;i< pack.data_length;i++)
	{
		packagedata[DATA_START_POSITION+i] = cmd.data[i]; //zapis data v package
	}
	int commandID = cmd.commandID;
	char commID_char[TYPE_FIELD_SIZE+1] = {};
	sprintf(commID_char,"%2i",commandID); //preobraz int v char   IDCMD

	char length_char[LENGTH_FIELD_SIZE+1] = {};
    sprintf(length_char,"%3i",pack.packagesize); //preobraz int v char  DLINA ZAGOLOVKA


	for(int i = 0;i < Package::LENGTH_FIELD_SIZE;i++) // zapis dlina package v dlinu package
	{
		packagedata[i] = length_char[i];
	}

	for(int i = 0;i < Package::TYPE_FIELD_SIZE;i++) // zapis id package v id package
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

/*
Package Package::ParseToPackage(char* inputmessage)
{
    Package package;
    Command cmd = Parse(inputmessage);
}
*/

#include "StdAfx.h"
#include "Package.h"


Package::Package(void)
{
}


Package::~Package(void)
{
}

Command Package::Parse(BYTE* inputmessage)
{
	BYTE bytetype = inputmessage[LENGTH_FIELD_SIZE];
	int commandtype = int(bytetype);
	Command command(commandtype);
	return command;
}

Package Package::Generate(Command cmd) //nam prichodit command  - preobraaz v package posulat
{
	int data_length = sizeof(cmd.data)/sizeof(BYTE); //dl data
	int packagesize = Package::DATA_START_POSITION + data_length; //dl package = slujebnoy inf + data
	BYTE * packagedata = new BYTE[packagesize]; //ves package
	

	for(int i=0;i< data_length;i++)
	{
		packagedata[DATA_START_POSITION+i] = cmd.data[i]; //zapis data v package
	}
	int commandID = cmd.commandID;
	char commID_char[3] = "  ";
	sprintf(commID_char,"%2i",commandID); //preobraz int v char   IDCMD

	char length_char[4] = "  ";
	sprintf(length_char,"%3i",packagesize); //preobraz int v char  DLINA ZAGOLOVKA

	for(int i = 0;i < Package::LENGTH_FIELD_SIZE;i++) // zapis dlina package v dlinu package
	{
		packagedata[i] = length_char[i];
	}

	for(int i = 0;i < Package::TYPE_FIELD_SIZE;i++) // zapis id package v id package
	{
		packagedata[i+LENGTH_FIELD_SIZE] = commID_char[i];
	}
	
	Package pack;
	pack.data = packagedata;//save v  package
	return pack;
}

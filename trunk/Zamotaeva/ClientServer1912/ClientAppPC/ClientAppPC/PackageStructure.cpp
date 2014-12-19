#include "stdafx.h"
#include "PackageStructure.h"
#include "MyCreator.h"



char * GeneratePackageData(PackageStructure packstructure)
{
	int data_length = packstructure.cmdstructure.data_len;
	packstructure.package_len = DATA_START_POSITION + data_length;

	char * packagedata = getCleanBuffer(packstructure.package_len+1);
	bzero(packagedata,packstructure.package_len);
	char * format = getCleanBuffer(11);
	//@dev: unsupportable
	sprintf(format,"%s%3d%s","%3d%2d",data_length,"%s");
	sprintf(packagedata,"%3d%2d%s%s",packstructure.package_len,packstructure.cmdstructure.CommandId,packstructure.cmdstructure.data,'\0');

	/*
	for(int i=0;i < packstructure.package_len; i++)
	{
		if(packagedata[i] == ' ')
		{
			packagedata[i] = '0';
		}

	}
	*/
	return packagedata;
}


struct PackageStructure CreatePackage(CommandStructure cmd)
{
	PackageStructure package;
	package.cmdstructure = cmd;
	return package;
}


CommandStructure Parse(char* inputmessage)
{
	int shift = 0;
	CommandStructure cmd;

	//parse package size
	char *c_packlen = getCleanBuffer(LENGTH_FIELD_SIZE);
	int i_packlen = 0;
	for(int i=0; i<LENGTH_FIELD_SIZE;i++)
	{
		c_packlen[i] =inputmessage[shift];
		shift++;
	}
	sscanf(c_packlen,"%d",&i_packlen);

	//move data to new array (remove garbage and decrease memory size)

	char* fixmessage;
	if(i_packlen > 0)
	{
		fixmessage = getCleanBuffer(i_packlen);
		for(int i = 0; i< i_packlen;i++)
		{
			char end_char =  (char)inputmessage[i];
			fixmessage[i] = end_char;
		}
	}
	else
	{
		CommandStructure BadPackCMD;
		BadPackCMD.CommandId = BAD_PACKAGE;
		return BadPackCMD;
	}

	//parse command type
	char *c_packtype = getCleanBuffer(TYPE_FIELD_SIZE);//(char*)malloc(sizeof(char)*TYPE_FIELD_SIZE);
	int i_cmdtype = 0;
	for(int i=0; i<TYPE_FIELD_SIZE;i++)
	{
		c_packtype[i] =fixmessage[shift];
		shift++;
	}
    sscanf(c_packtype,"%d",&i_cmdtype);
	cmd.CommandId =  i_cmdtype;

	//parse package data
	int data_size = i_packlen - shift;
	char *c_packdata;
	if(data_size > 0)
	{
		c_packdata = getCleanBuffer(data_size +1);
	}
	for(int i = 0;i<data_size;i++)
	{
		c_packdata[i] = fixmessage[shift];
		shift++;
	}
	//c_packdata[data_size] = '\0';
	cmd.data = c_packdata;
	cmd.data_len = data_size;
	return cmd;
}

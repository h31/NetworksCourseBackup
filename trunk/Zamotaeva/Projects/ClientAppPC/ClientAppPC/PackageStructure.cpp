#include "stdafx.h"
#include "PackageStructure.h"



char * GeneratePackageData(PackageStructure packstructure)
{
	int data_length = packstructure.cmdstructure.data_len;
	packstructure.package_len = DATA_START_POSITION + data_length;

	char * packagedata = (char*)malloc(sizeof(char)*packstructure.package_len);
	bzero(packagedata,packstructure.package_len);
	char * format = (char*)malloc(sizeof(char)*11);
	sprintf(format,"%s%3d%s","%3d%2d",data_length,"%s");
	sprintf(packagedata,"%3d%2d%s",packstructure.package_len,packstructure.cmdstructure.CommandId,packstructure.cmdstructure.data);

	for(int i=0;i < packstructure.package_len; i++)
	{
		if(packagedata[i] == ' ')
		{
			packagedata[i] = '0';
		}

	}
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
	char *c_packlen = (char*)malloc(sizeof(char)*LENGTH_FIELD_SIZE);
	int i_packlen = 0;
	for(int i=0; i<LENGTH_FIELD_SIZE;i++)
	{
		c_packlen[i] =inputmessage[shift];
		shift++;
	}
	sscanf(c_packlen,"%d",&i_packlen);

	//parse command type
	char *c_packtype = (char*)malloc(sizeof(char)*TYPE_FIELD_SIZE);
	int i_cmdtype = 0;
	for(int i=0; i<TYPE_FIELD_SIZE;i++)
	{
		c_packtype[i] =inputmessage[shift];
		shift++;
	}
    sscanf(c_packtype,"%d",&i_cmdtype);
	cmd.CommandId =  i_cmdtype;

	//parse package data
	int data_size = i_packlen - shift+1;
	char *c_packdata;
	if(data_size > 0)
	{
		c_packdata = (char*)malloc(sizeof(char)*data_size);
	}
	for(int i = 0;i<data_size;i++)
	{
		c_packdata[i] = inputmessage[shift];
		shift++;
	}
	cmd.data = c_packdata;
	cmd.data_len = data_size;
	return cmd;
}

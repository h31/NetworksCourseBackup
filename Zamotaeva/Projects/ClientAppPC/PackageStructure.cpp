#include "PackageStructure.h"
#include "stdafx.h"
char * GeneratePackageData(PackageStructure packstructure)
{
	int data_length = packstructure.cmdstructure.data_len;
	packstructure.package_len = DATA_START_POSITION + data_length;

	char * packagedata = new char[packstructure.package_len];
	bzero(packagedata,packstructure.package_len);

	sprintf(packagedata,"%3d%2d%15s%s",packstructure.package_len,packstructure.cmdstructure.CommandId,packstructure.sender,packstructure.cmdstructure.data);
	/*
	for(int i=0;i< data_length;i++)
	{
		packagedata[DATA_START_POSITION+i] = packstructure.cmdstructure.data[i]; //zapis data v package
	}
	*/
	for(int i=0;i < packstructure.package_len; i++)
	{
		if(packagedata[i] == ' ')
		{
			packagedata[i] = '0';
		}
	}
	return packagedata;
}


PackageStructure CreatePackage(CommandStructure cmd, char* sender)
{
	PackageStructure package;
	package.cmdstructure = cmd;
	package.sender = sender;
	return package;
}


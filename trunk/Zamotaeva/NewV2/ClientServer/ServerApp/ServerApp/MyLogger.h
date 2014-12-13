#pragma once
#include <iostream>
#include "AppColorTheme.h"
using namespace std;
class MyLogger
{
public:
	MyLogger(void);
	~MyLogger(void);

	static void WriteSuccessNetworkEvent(char * text)
	{
		AppColorTheme::setSuccessColor();
		printf("Success:\t%s\n",text);
		AppColorTheme::setNormalColor();
	}
	static void WriteSuccessNetworkEvent(char * text,char *object)
	{
		AppColorTheme::setSuccessColor();
		printf("Success:\t%s  - %s\n",text,object);
		AppColorTheme::setNormalColor();
	}

	static void WriteFailNetworkEvent(char * text)
	{
		AppColorTheme::setErrorColor();
		printf("%s\n",text);
		AppColorTheme::setNormalColor();
	}

	static void WriteFailEvent(string text)
	{
		AppColorTheme::setErrorColor();
		printf("%s\n",(char*)text.c_str());
		AppColorTheme::setNormalColor();
	}

	static void WriteMessage(char * text)
	{
		AppColorTheme::setMessageColor();
		printf("message:\t%s\n",text);
		AppColorTheme::setNormalColor();
	}


	static void WritePackageInfo(char * text,bool issend)
	{
		
		if(issend == true)
		{
			AppColorTheme::setPackageColor();
			printf("send package:\t%s\n",text);
		}
		else
		{
			AppColorTheme::setPackageColor();
			printf("receive package:\t%s\n",text);
		}
		
		AppColorTheme::setNormalColor();
	}

	static void WritePackageInfo(char * text,char * status)
	{
		AppColorTheme::setPackageColor();
		printf("%s package:\t%s\n",status,text);
		AppColorTheme::setNormalColor();
	}

	static void WriteObjectInfo(char *object,char * text)
	{
		AppColorTheme::setWarningColor();
		cout<< object<<":\t";
		cout<< text<<endl;
		AppColorTheme::setNormalColor();
	}
};


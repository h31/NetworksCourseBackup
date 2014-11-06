#pragma once
#include <iostream>
#include "AppColorTheme.h"
using namespace std;
class MyLogger //func vivoda
{
public:
	MyLogger(void);
	~MyLogger(void);

	static void WriteSuccessNetworkEvent(char * text)
	{
		AppColorTheme::setSuccessColor();
		cout<<"Success:\t";
		cout<< text<<endl;
		AppColorTheme::setNormalColor();
	}

	static void WriteFailNetworkEvent(char * text)
	{
		AppColorTheme::setErrorColor();
		cout<<"Fail:\t";
		cout<< text<<endl;
		AppColorTheme::setNormalColor();
	}

	static void WriteMessage(char * text)
	{
		AppColorTheme::setMessageColor();
		cout<<"message:\t";
		cout<< text<<endl;
		AppColorTheme::setNormalColor();
	}


	static void WriteMessage(BYTE * text)
	{
		AppColorTheme::setMessageColor();
		cout<<"message:\t";
		cout<< text<<endl;
		AppColorTheme::setNormalColor();
	}

	static void WriteAdv(char * text)
	{
		AppColorTheme::setAdvColor();
		cout<< text<<endl;
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


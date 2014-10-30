#pragma once
#include <iostream>
using namespace std;
class MyLogger //func vivoda
{
public:
	MyLogger(void);
	~MyLogger(void);

	static void WriteSuccessEvent(char * text)
	{
		cout<<"Success:\t";
		cout<< text<<endl;
	}

	static void WriteFailEvent(char * text)
	{
		cout<<"Fail:\t";
		cout<< text<<endl;
	}
};


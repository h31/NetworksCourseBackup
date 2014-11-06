#pragma once
#include <Windows.h>
class AppColorTheme
{
public:

	static void InitConsoleColor()
	{
		system("color F0");
	}
	static void setWarningColor()
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
		SetConsoleTextAttribute(hStdout, 246);
	}
	static void setErrorColor()
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
		SetConsoleTextAttribute(hStdout, 244);
	}
	static void setNormalColor()
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
		SetConsoleTextAttribute(hStdout, 240);
	}
	static void setSuccessColor()
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
		SetConsoleTextAttribute(hStdout, 242);
	}

	static void setMessageColor()
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
		SetConsoleTextAttribute(hStdout, 249);
	}

	static void setAdvColor()
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
		SetConsoleTextAttribute(hStdout, 248);
	}

};


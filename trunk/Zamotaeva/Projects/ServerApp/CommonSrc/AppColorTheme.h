#pragma once
#ifdef _WIN32
#include <Windows.h>
#elif __linux
#endif
class AppColorTheme
{
public:

	static void InitConsoleColor()
	{
        #ifdef _WIN32
		system("color F0");
        #endif
	}
	static void setWarningColor()
	{
        #ifdef _WIN32
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
		SetConsoleTextAttribute(hStdout, 246);
        #endif
	}
	static void setErrorColor()
	{
        #ifdef _WIN32
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
		SetConsoleTextAttribute(hStdout, 244);
        #endif
	}
	static void setNormalColor()
	{
        #ifdef _WIN32
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
		SetConsoleTextAttribute(hStdout, 240);
        #endif
	}
	static void setSuccessColor()
	{
        #ifdef _WIN32
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
		SetConsoleTextAttribute(hStdout, 242);
        #endif
	}

	static void setMessageColor()
	{
        #ifdef _WIN32
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
		SetConsoleTextAttribute(hStdout, 249);
        #endif
	}

	static void setAdvColor()
	{
        #ifdef _WIN32
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
		SetConsoleTextAttribute(hStdout, 248);
        #endif
	}

};


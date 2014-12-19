#include "stdafx.h"
#include "AppTheme.h"
#include "MyCreator.h"
#include <ctype.h>
void InitConsoleColor()
{
#ifdef _WIN32
	system("color F0");
#endif
}

void setWarningColor()
{
#ifdef _WIN32
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute(hStdout, 246);
#endif
}

void setErrorColor()
{
#ifdef _WIN32
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute(hStdout, WINCONSOLE_RED_COLOR);
#endif
}

void setNormalColor()
{
#ifdef _WIN32
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute(hStdout, 240);
#endif
}

void setSuccessColor()
{
#ifdef _WIN32
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute(hStdout, WINCONSOLE_GREEN_COLOR);
#endif
}

void ClearConsole()
{
	setNormalColor();
#ifdef _WIN32
	system("cls");
#endif
	setNormalColor();
}

void setMessageColor()
{
#ifdef _WIN32
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute(hStdout, 249);
#endif
}

void setPackageColor()
{
#ifdef _WIN32
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute(hStdout, WINCONSOLE_GRAY_COLOR);
#endif
}


void PrintfInfoTitleText(char* login, int Nonline,int Nmessages)
{
	int login_len = strlen(login)+3;
	int real_size = 0;
	printf("[");
	for(int i = 0; i<login_len;i++)
	{
		char symb = checkStrongSymbol(login[i]);
		if(symb != ' ')
		{
			printf("%c",symb);
		}
		else
		{
			break;
		}

	}
	printf("]");
	char *login_fix = getCleanBuffer(real_size+1);
	for(int i = 0;i<real_size;i++)
	{
		login_fix[i] = (char)login[i];
	}
	login_fix[real_size -1 ] = '\n';

	char *messageinfo = "new messages:";
	char *onlineinfo = "online:";
	int mes_len = strlen(messageinfo)+3;
	int onl_len = strlen(onlineinfo);
	char *title = getCleanBuffer(login_len +mes_len+1);
	sprintf(title,",%s%3d\t%s%3d\n\n",messageinfo,Nmessages,onlineinfo,Nonline);
	printf ("%s",title);
}


void setConsoleTitle()
{
#ifdef _WIN32
	SetConsoleTitle(L"Client");
#endif
}

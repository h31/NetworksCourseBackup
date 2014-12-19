#ifndef _APPCOLORTHEME_H
#define _APPCOLORTHEME_H


#ifdef _WIN32
#include <Windows.h>
#elif __linux
#endif



const int WINCONSOLE_RED_COLOR = 244;
const int WINCONSOLE_GRAY_COLOR = 248;
const int WINCONSOLE_GREEN_COLOR = 242;

void InitConsoleColor();
void ClearConsole();
void setWarningColor();
void setErrorColor();
void setNormalColor();
void setSuccessColor();

void setMessageColor();
void setPackageColor();
void PrintfInfoTitleText(char* login, int Nonline,int Nmessages);
void setConsoleTitle();

#endif
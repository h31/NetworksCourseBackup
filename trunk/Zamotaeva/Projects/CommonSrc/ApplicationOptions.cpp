#include "stdafx.h"
#include "ApplicationOptions.h"
#include "AppColorTheme.h"
#ifdef _WIN32
#include <Windows.h>
#endif


void ApplicationOptions::setServerAppOptions()
{

#ifdef _WIN32
	SetConsoleTitleW(L"Server");
#elif __linux
    char esc_start[] = { 0x1b, ']', '0', ';', 0 };
    char esc_end[] = { 0x07, 0 };
    printf("%s%s%s",esc_start,"Server",esc_end);
    //cout << esc_start <<titleMsg<< esc_end;
#endif
	AppColorTheme::InitConsoleColor();

}

void ApplicationOptions::setClientAppOptions()
{
#ifdef _WIN32
    SetConsoleTitleW(L"Client");
#elif __linux
    char esc_start[] = { 0x1b, ']', '0', ';', 0 };
    char esc_end[] = { 0x07, 0 };
    printf("%s%s%s",esc_start,"Client",esc_end);
    //cout << esc_start <<titleMsg<< esc_end;
#endif
	AppColorTheme::InitConsoleColor();
}

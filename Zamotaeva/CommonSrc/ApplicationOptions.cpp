#include "StdAfx.h"
#include "ApplicationOptions.h"
#include "AppColorTheme.h"
#include <Windows.h>


void ApplicationOptions::setServerAppOptions()
{

	SetConsoleTitleW(L"Server");
	AppColorTheme::InitConsoleColor();

}

void ApplicationOptions::setClientAppOptions()
{
	SetConsoleTitleW(L"Client");
	AppColorTheme::InitConsoleColor();
}

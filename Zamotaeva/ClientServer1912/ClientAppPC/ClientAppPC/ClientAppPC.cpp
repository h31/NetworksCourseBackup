#include "stdafx.h"
#include <stdlib.h>

#include "MyClient.h"
#include "PackageStructure.h"
#include "MessageStructure.h"
#include "GetCurrentTime.h"
#include "CommandFactory.h"
#include "CrossPlatformDefines.h"
#include "AppTheme.h"




int main(int argc, _TCHAR* argv[])
{

	InitConsoleColor();
	setConsoleTitle();

    SetUpLib();
	if(CurrentRegime ==TCPregime)
	{
		printf("TCP\n");
		runTCP();
	}
	else
	{
		if(CurrentRegime == UDPregime)	
		{
			printf("UDP\n");
			runUDP();
		}
		else
		{
			WriteFailEvent("set correct connection mode (TCP/UDP)\n");
		}
	}
    CleanLib();
    return 0;
}

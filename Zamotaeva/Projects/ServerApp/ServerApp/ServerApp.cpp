// ServerApp.cpp:

#include "stdafx.h"
#include "MyServer.h"

#include "../CommonSrc/ApplicationOptions.h"

int main(int argc, _TCHAR* argv[])
{
	ApplicationOptions::setServerAppOptions();
	MyServer* server = new MyServer();
	server->SetupLibrary();
    if(CurrentRegime == TCPregime)
    {
        server->runTCP();
    }
    if(CurrentRegime == UDPregime)
    {
		//server->runUDP();
    }
	server->CleanUpLibrary();
}


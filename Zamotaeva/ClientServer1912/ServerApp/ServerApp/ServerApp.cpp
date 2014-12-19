// ServerApp.cpp:

#include "stdafx.h"
#include "ApplicationOptions.h"
#include "MyServer.h"


int main(int argc, _TCHAR* argv[])
{
	ApplicationOptions::setServerAppOptions();
	MyServer* server = new MyServer();
	server->SetupLibrary();
    if(CurrentRegime == TCPregime)
    {
		printf("TCP\n");
        server->runTCP();
    }
    if(CurrentRegime == UDPregime)
    {
		printf("UDP\n");
		server->runUDP();
    }
	server->CleanUpLibrary();
}


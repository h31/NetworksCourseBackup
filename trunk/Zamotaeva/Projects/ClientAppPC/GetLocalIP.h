#ifndef _GETLOCALIP_H_
#define _GETLOCALIP_H_

#include "CrossPlatformDefines.h"

#ifdef __linux__
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
#elif _WIN32
    #include <WinSock2.h>
    #include <windows.h>

    typedef SOCKET mysocket;

#else

#endif

char* getIPAddress()
{
	char ac[80];	//computer name
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) 
	{
		return "";
	}
	struct hostent *phe = gethostbyname(ac);
	if (phe == 0) 
	{
		return "";
	}
    int Octet1 = 0;
    int Octet2 = 0;
    int Octet3 = 0;
    int Octet4 = 0;

#ifdef _WIN32
	Octet1 = ((struct in_addr *)(phe->h_addr))->S_un.S_un_b.s_b1;
    Octet2 = ((struct in_addr *)(phe->h_addr))->S_un.S_un_b.s_b2;
    Octet3 = ((struct in_addr *)(phe->h_addr))->S_un.S_un_b.s_b3;
    Octet4 = ((struct in_addr *)(phe->h_addr))->S_un.S_un_b.s_b4;
#elif __linux__
    //4in_addr* addr = ((struct in_addr *)(phe->h_addr));
    //Octet1 = addr->s_addr;
#endif

	char *result = new char[16];
	sprintf(result,"%3i.%3i.%3i.%3i",Octet1,Octet2,Octet3,Octet4);
	return result;
	
}
#endif

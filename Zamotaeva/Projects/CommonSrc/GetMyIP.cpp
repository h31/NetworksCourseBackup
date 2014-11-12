#include "stdafx.h"
#include "GetMyIP.h"
#include <CrossPlatformDefines.h>

GetMyIP::GetMyIP(void)
{
}


GetMyIP::~GetMyIP(void)
{
}

char *GetMyIP::getIPAddress()
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

#if _WIN32
	Octet1 = ((struct in_addr *)(phe->h_addr))->S_un.S_un_b.s_b1;
    Octet2 = ((struct in_addr *)(phe->h_addr))->S_un.S_un_b.s_b2;
    Octet3 = ((struct in_addr *)(phe->h_addr))->S_un.S_un_b.s_b3;
    Octet4 = ((struct in_addr *)(phe->h_addr))->S_un.S_un_b.s_b4;
#elif __linux
    Octet1 = 0;
    Octet2 = 0;
    Octet3 = 0;
    Octet4 = 0;
#endif

	char *result = new char[16];
	sprintf(result,"%3i.%3i.%3i.%3i",Octet1,Octet2,Octet3,Octet4);
	return result;

}

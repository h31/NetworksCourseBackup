#include "MyClient.h"
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
	#pragma comment(lib, "WS2_32.lib")
#endif





sockaddr_in getDefaultAddressParam()
{
	sockaddr_in address;

	address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons( DEFAULT_CONNECTION_PORT );
	return address;
}

sockaddr_in getListenAddressParam()
{
	sockaddr_in address;
	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( DEFAULT_CONNECTION_PORT );
	return address;
}


void SetUpLib()
{
	#ifdef _WIN32
	WSADATA wsaData;
    int errorcode = WSAStartup(MAKEWORD(2,2),&wsaData);
	#endif
}
mysocket createConnection()
{

    mysocket sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock == INVALID_SOCKET)
	{
        printf("invalid socket");
	}
	sockaddr_in servaddr = getDefaultAddressParam();
	connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));
	return sock;
}
void SendPackage(mysocket s,PackageStructure package,sockaddr_in servaddr)
{
	int len = package.package_len;
	char * data = GeneratePackageData(package);
    int sendsize = sendto(s, data ,len , 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
    printf("send bytes: %d",sendsize);
}

void SendPackage(mysocket s,PackageStructure package)
{
	sockaddr_in servaddr = getDefaultAddressParam();
	
	char * data = GeneratePackageData(package);
	int len = strlen(data);
	int sendsize = sendto(s, data ,len , 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
    printf("send bytes: %d",sendsize);

}

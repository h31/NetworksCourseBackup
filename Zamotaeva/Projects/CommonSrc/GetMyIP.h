#pragma once


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
class GetMyIP
{
public:
	GetMyIP(void);
	~GetMyIP(void);
	static char* getIPAddress();
};


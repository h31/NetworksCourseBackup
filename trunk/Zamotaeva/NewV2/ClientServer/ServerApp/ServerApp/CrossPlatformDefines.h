#pragma once




#ifdef _WIN32
#include <WinSock2.h>
#include <windows.h>
typedef SOCKET mysocket;
typedef DWORD returnType; //win thread
typedef int sock_len;
#elif __linux
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
typedef int mysocket;
typedef void * returnType;	//linux thread
typedef socklen_t sock_len;
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET          (mysocket)(~0)
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR            (-1)
#endif

#ifndef _TCHAR
#define _TCHAR  wchar_t
#endif


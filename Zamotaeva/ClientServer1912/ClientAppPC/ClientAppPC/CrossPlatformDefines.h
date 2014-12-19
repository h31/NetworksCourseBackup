#ifndef CROSSPLATFORMDEFINES_H
#define CROSSPLATFORMDEFINES_H


#ifndef INVALID_SOCKET
#define INVALID_SOCKET          (mysocket)(~0)
#endif

/*
#ifndef SOCKET_ERROR
#define SOCKET_ERROR            (-1)
#endif
*/

#ifndef _TCHAR
#define _TCHAR  wchar_t
#endif


#endif // CROSSPLATFORMDEFINES_H

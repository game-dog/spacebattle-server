#ifndef __SOCKET_UTIL_H__
#define __SOCKET_UTIL_H__

#include <WinSock2.h>
#include "TCPSocket.h"

class SocketUtil {
public:
	static bool Init();
	static void CleanUp();

	static void ReportError(const char* inOperationDesc);
	static int GetLastError();

	static TCPSocketPtr CreateTCPSocket();

private:
};

#endif
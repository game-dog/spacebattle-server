#ifndef __SOCKET_UTIL_H__
#define __SOCKET_UTIL_H__

#include <WinSock2.h>
#include "TCPSocket.h"

class IOCP;

class SocketUtil {
public:
	static bool Init();
	static void CleanUp();

	static void ReportError(const char* inOperationDesc);
	static int GetLastError();

	static std::shared_ptr<TCPSocket> CreateTCPSocket();
	static std::shared_ptr<IOCP> CreateIOCP();

private:
};

#endif
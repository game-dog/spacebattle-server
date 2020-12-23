#ifndef __IOCP_H__
#define __IOCP_H__

#include <Winsock2.h>

#include "TCPSocket.h"
#include "NetworkManager.h"

class SocketUtil;

class IOCP {
public:
	void ConnectSockToIOCP(std::shared_ptr<TCPSocket> clntSock, LPIOCP_KEY_DATA pKeyData);


	HANDLE GetIOCP() { return mComPort; }

private:
	friend class SocketUtil;
	IOCP(HANDLE inComPort) : mComPort(inComPort) { }
	HANDLE mComPort;
};

#endif
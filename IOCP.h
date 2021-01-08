#ifndef __IOCP_H__
#define __IOCP_H__

#include <Winsock2.h>

#include "TCPSocket.h"
#include "NetworkManager.h"

class SocketUtil;

typedef struct {
	std::shared_ptr<TCPSocket> pClntSock;
	SocketAddress clntAddr;
	std::string id;
} IOCP_KEY_DATA, * LPIOCP_KEY_DATA;

class IOCP {
public:
	void ConnectSockToIOCP(std::shared_ptr<TCPSocket> clntSock, SocketAddress &sockAddr);
	int32_t GetCompletion(LPIOCP_KEY_DATA& pKeyData, LPIO_DATA& pIOData);

	HANDLE GetIOCP() { return mComPort; }

private:
	friend class SocketUtil;
	IOCP(HANDLE inComPort) : mComPort(inComPort) { }
	HANDLE mComPort;
};

#endif
#ifndef __TCP_SOCKET_H__
#define __TCP_SOCKET_H__

#include "SocketAddress.h"
#include <memory>

class TCPSocket {
public:
	~TCPSocket() { closesocket(mSocket); }
	int Connect(const SocketAddress& inAddress);
	int Bind(const SocketAddress& inToAddress);
	int Listen(int inBackLog);
	std::shared_ptr<TCPSocket> Accept(SocketAddress& inFromAddress);
	int Send(const void* inData, int inLen);
	int Receive(void* inBuffer, int inLen);

	SOCKET GetSocket() { return mSocket; }

private:
	friend class SocketUtil;
	TCPSocket(SOCKET inSocket) : mSocket(inSocket) {}
	SOCKET mSocket;
};

#endif
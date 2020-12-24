#ifndef __TCP_SOCKET_H__
#define __TCP_SOCKET_H__

#include <memory>

#include "SocketAddress.h"

enum {
	MODE_READ,
	MODE_WRITE
};

typedef struct {
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[1024];
	int rwMode;
} IO_DATA, *LPIO_DATA;

class TCPSocket {
public:
	~TCPSocket() { closesocket(mSocket); }
	int Connect(const SocketAddress& inAddress);
	int Bind(const SocketAddress& inToAddress);
	int Listen(int inBackLog);
	std::shared_ptr<TCPSocket> Accept(SocketAddress& inFromAddress);
	int Send(const void* inData, int inLen);
	int Receive();

	SOCKET GetSocket() { return mSocket; }

private:
	friend class SocketUtil;
	TCPSocket(SOCKET inSocket) : mSocket(inSocket) {}
	SOCKET mSocket;

	IO_DATA ioData;
};

#endif
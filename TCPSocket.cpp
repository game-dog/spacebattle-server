#include "TCPSocket.h"
#include "SocketUtil.h"

int TCPSocket::Connect(const SocketAddress& inAddress) {
	int err = connect(mSocket, &inAddress.mSockAddr, inAddress.GetSize());
	if (err < 0) {
		// TODO: 俊矾 贸府
	}
	return 0;
}

int TCPSocket::Bind(const SocketAddress& inToAddress) {
	int err = bind(mSocket, &inToAddress.mSockAddr, inToAddress.GetSize());
	if (err == SOCKET_ERROR) {
		// TODO: 俊矾 贸府
	}
	return 0;
}

int TCPSocket::Listen(int inBackLog) {
	int err = listen(mSocket, inBackLog);
	if (err == SOCKET_ERROR) {
		// TODO: 俊矾 贸府
	}
	return 0;
}

TCPSocketPtr TCPSocket::Accept(SocketAddress& inFromAddress) {
	socklen_t length = inFromAddress.GetSize();
	SOCKET sock = accept(mSocket, &inFromAddress.mSockAddr, &length);
	if (sock == INVALID_SOCKET) {
		SocketUtil::ReportError("TCPSocket::Accept");
		return nullptr;
	}
	return TCPSocketPtr(new TCPSocket(sock));
}

int TCPSocket::Send(const void* inData, int inLen) {
	return 0;
}

int TCPSocket::Receive(void* inBuffer, int inLen) {
	return 0;
}
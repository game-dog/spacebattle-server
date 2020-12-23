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

std::shared_ptr<TCPSocket> TCPSocket::Accept(SocketAddress& inFromAddress) {
	socklen_t length = inFromAddress.GetSize();
	SOCKET sock = accept(mSocket, &inFromAddress.mSockAddr, &length);
	if (sock == INVALID_SOCKET) {
		SocketUtil::ReportError("TCPSocket::Accept");
		return nullptr;
	}
	return std::shared_ptr<TCPSocket>(new TCPSocket(sock));
}

int TCPSocket::Send(const void* inData, int inLen) {
	return 0;
}

int TCPSocket::Receive(void* inBuffer, int inLen) {
	//DWORD recvBytes, flags = 0;
	//int err = WSARecv(mSocket, &(ioData->wsaBuf), 1, &recvBytes, &flags, &(ioData->overlapped), NULL);
	//if (err == 0) {
	//	SocketUtil::ReportError("TCPSocket::Receive");
	//	return -SocketUtil::GetLastError();
	//}
	//return recvBytes;
}
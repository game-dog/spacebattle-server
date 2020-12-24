#include "TCPSocket.h"
#include "SocketUtil.h"

int TCPSocket::Connect(const SocketAddress& inAddress) {
	int err = connect(mSocket, &inAddress.mSockAddr, inAddress.GetSize());
	if (err < 0) {
		SocketUtil::ReportError("TCPSocket::Connect");
		return -SocketUtil::GetLastError();
	}
	return 0;
}

int TCPSocket::Bind(const SocketAddress& inToAddress) {
	int err = bind(mSocket, &inToAddress.mSockAddr, inToAddress.GetSize());
	if (err == SOCKET_ERROR) {
		SocketUtil::ReportError("TCPSocket::Bind");
		return -SocketUtil::GetLastError();
	}
	return 0;
}

int TCPSocket::Listen(int inBackLog) {
	int err = listen(mSocket, inBackLog);
	if (err == SOCKET_ERROR) {
		SocketUtil::ReportError("TCPSocket::Listen");
		return -SocketUtil::GetLastError();
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

int TCPSocket::Receive() {
	DWORD recvBytes, flags = 0;

	memset(&(ioData.overlapped), 0, sizeof(OVERLAPPED));
	ioData.wsaBuf.buf = ioData.buffer;
	ioData.wsaBuf.len = sizeof(ioData.buffer);
	ioData.rwMode = MODE_READ;

	int err = WSARecv(mSocket, &(ioData.wsaBuf), 1, &recvBytes, &flags, &(ioData.overlapped), NULL);
	if (err == 0) {
		SocketUtil::ReportError("TCPSocket::Receive");
		return -SocketUtil::GetLastError();
	}

	return recvBytes;
}
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
	DWORD sentBytes = inLen, flags = 0;

	memset(&(ioData.overlapped), 0, sizeof(OVERLAPPED));
	memcpy(reinterpret_cast<void*>(ioData.buffer), inData, inLen);
	ioData.wsaBuf.buf = ioData.buffer;
	ioData.wsaBuf.len = sizeof(ioData.buffer);
	ioData.rwMode = MODE_WRITE;

	// TODO: 데이터가 1024바이트를 넘어갈 수 있으므로, 다 보낼때 까지 지속적인 send 호출 필요
	int err = WSASend(mSocket, &(ioData.wsaBuf), 1, &sentBytes, flags, &(ioData.overlapped), NULL);
	if (err == SOCKET_ERROR) {
		int lastErr = SocketUtil::GetLastError();
		if (lastErr != WSA_IO_PENDING) {
			SocketUtil::ReportError("TCPSocket::Send");
			return -lastErr;
		}
	}

	return sentBytes;
}

int TCPSocket::Receive() {
	DWORD recvBytes, flags = 0;

	memset(&(ioData.overlapped), 0, sizeof(OVERLAPPED));
	ioData.wsaBuf.buf = ioData.buffer;
	ioData.wsaBuf.len = sizeof(ioData.buffer);
	ioData.rwMode = MODE_READ;

	// TODO: 데이터가 1024바이트를 넘어갈 수 있으므로, 다 받을때 까지 지속적인 recv 호출 필요
	int err = WSARecv(mSocket, &(ioData.wsaBuf), 1, &recvBytes, &flags, &(ioData.overlapped), NULL);
	if (err == SOCKET_ERROR) {
		int lastErr = SocketUtil::GetLastError();
		if (lastErr != WSA_IO_PENDING) {
			SocketUtil::ReportError("TCPSocket::Receive");
			return -lastErr;
		}
	}

	return recvBytes;
}
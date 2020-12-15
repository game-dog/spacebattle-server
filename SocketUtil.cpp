#include "SocketUtil.h"

bool SocketUtil::Init() {
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != NO_ERROR) {
		// TODO: 俊矾 贸府
	}
	return true;
}

void SocketUtil::CleanUp() {
	WSACleanup();
}

void SocketUtil::ReportError(const char* inOperationDesc) {
	LPVOID lpMsgBuf;
	DWORD errNum = GetLastError();
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);
	// TODO: 俊矾 贸府
}

int SocketUtil::GetLastError() {
	return WSAGetLastError();
}

TCPSocketPtr SocketUtil::CreateTCPSocket() {
	SOCKET sock = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, 0, NULL, WSA_FLAG_OVERLAPPED);
	if (sock == INVALID_SOCKET) {
		SocketUtil::ReportError("SocketUtil::CreateTCPSocket");
		return nullptr;
	}
	return TCPSocketPtr(new TCPSocket(sock));
}
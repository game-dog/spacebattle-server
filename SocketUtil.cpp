#include "SocketUtil.h"
#include "IOCP.h"

bool SocketUtil::Init() {
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != NO_ERROR) {
		// TODO: 에러 처리 코드
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
	LOG("Error %s: %d- %s", inOperationDesc, errNum, lpMsgBuf);
}

int SocketUtil::GetLastError() {
	return WSAGetLastError();
}

std::shared_ptr<TCPSocket> SocketUtil::CreateTCPSocket() {
	SOCKET sock = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, 0, NULL, WSA_FLAG_OVERLAPPED);

	// TIME-WAIT disable debugging option
	int option = TRUE;
	socklen_t optlen = sizeof(option);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&option, optlen);
	// .............

	if (sock == INVALID_SOCKET) {
		SocketUtil::ReportError("SocketUtil::CreateTCPSocket");
		return nullptr;
	}
	return std::shared_ptr<TCPSocket>(new TCPSocket(sock));
}

std::shared_ptr<IOCP> SocketUtil::CreateIOCP() {
	HANDLE comPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (comPort == NULL) {
		SocketUtil::ReportError("SocketUtil::CreateIOCP");
		return nullptr;
	}
	return std::shared_ptr<IOCP>(new IOCP(comPort));
}
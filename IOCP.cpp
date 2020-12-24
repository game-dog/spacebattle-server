#include "IOCP.h"
#include "SocketUtil.h"

void IOCP::ConnectSockToIOCP(std::shared_ptr<TCPSocket> clntSock, LPIOCP_KEY_DATA pKeyData) {
	HANDLE hIOCP = CreateIoCompletionPort((HANDLE)clntSock->GetSocket(), mComPort, (ULONG_PTR)pKeyData, 0);
	if (hIOCP == NULL) {
		SocketUtil::ReportError("IOCP::ConnectSockToIOCP");
		//return -SocketUtil::GetLastError();
	}
}

void IOCP::GetCompletion(LPIOCP_KEY_DATA& pKeyData, LPIO_DATA& pIOData) {
	BOOL err;
	DWORD bytesTrans;

	err = GetQueuedCompletionStatus(mComPort, &bytesTrans, (PULONG_PTR)&pKeyData, (LPOVERLAPPED*)&pIOData, INFINITE);
	if (err == FALSE) {
		SocketUtil::ReportError("IOCP::GetCompletion");
		//return -SocketUtil::GetLastError();
	}
}
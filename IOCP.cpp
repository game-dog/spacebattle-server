#include "IOCP.h"
#include "SocketUtil.h"

void IOCP::ConnectSockToIOCP(std::shared_ptr<TCPSocket> clntSock, LPIOCP_KEY_DATA pKeyData) {
	HANDLE hIOCP = CreateIoCompletionPort((HANDLE)clntSock->GetSocket(), mComPort, (DWORD)pKeyData, 0);
	if (hIOCP == NULL) {
		SocketUtil::ReportError("IOCP::ConnectSockToIOCP");
		//return -SocketUtil::GetLastError();
	}
}
#include "NetworkManager.h"
#include "IOCP.h"

void NetworkManager::Init() {
	GetSystemInfo(&mSysInfo);
}

void NetworkManager::OpenSockets() {
	std::shared_ptr<IOCP> pIOCP = SocketUtil::CreateIOCP();

	std::vector<std::thread> compRoutine;
	for (int i = 0; i < mSysInfo.dwNumberOfProcessors; ++i) {
		compRoutine.push_back(std::thread(ProcessLoginPacket, pIOCP));
	}

	SocketAddress sa(INADDR_ANY, 9001);

	std::shared_ptr<TCPSocket> pSock = SocketUtil::CreateTCPSocket();
	pSock->Bind(sa);
	pSock->Listen(15);

	std::thread acceptThr(AcceptThread, pSock, pIOCP);

	for (int i = 0; i < mSysInfo.dwNumberOfProcessors; ++i) {
		compRoutine[i].join();
	}
	acceptThr.join();

	// TODO: 채팅 소켓 개방
	// TODO: 플레이 소켓 개방
}

void NetworkManager::AcceptThread(std::shared_ptr<TCPSocket> pSock, std::shared_ptr<IOCP> pIOCP) {
	while (1) {
		SocketAddress clntAddr;
		std::shared_ptr<TCPSocket> pClntSock(pSock->Accept(clntAddr));

		LPIOCP_KEY_DATA pKeyData = new IOCP_KEY_DATA;
		pKeyData->pClntSock = pClntSock;
		pKeyData->clntAddr = clntAddr;
		pIOCP->ConnectSockToIOCP(pClntSock, pKeyData);

		LPIO_DATA ioData = new IO_DATA;
		memset(&(ioData->overlapped), 0, sizeof(OVERLAPPED));
		ioData->wsaBuf.buf = ioData->buffer;
		ioData->wsaBuf.len = 1024;
		// ioData->rwMode = 1;

		// TODO: TCPSocket 클래스 recv 함수 작성
		// pClntSock->Receive();
	}
}

void NetworkManager::ProcessLoginPacket(std::shared_ptr<IOCP> pIOCP) {
	DWORD bytesTrans;

	LPIOCP_KEY_DATA pKeyData;
	LPIO_DATA ioData;

	while (1) {
		GetQueuedCompletionStatus(pIOCP->GetIOCP(), &bytesTrans, (LPDWORD)&pKeyData, (LPOVERLAPPED*)&ioData, INFINITE);

		// TODO: 인증 후, client proxy 추가

		//InputBitStream ibs((uint8_t*)ioData->buffer, 1024);
		//char id[20], pw[20];
		//ibs.ReadBytes(id, 20);
		//ibs.ReadBytes(pw, 20);

		//LOG(id);
		//LOG(pw);
	}
}
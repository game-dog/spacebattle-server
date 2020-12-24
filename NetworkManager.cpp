#include "NetworkManager.h"
#include "IOCP.h"

void NetworkManager::Init() {
	GetSystemInfo(&mSysInfo);
}

void NetworkManager::OpenSockets() {
	std::shared_ptr<IOCP> pIOCP = SocketUtil::CreateIOCP();

	std::vector<std::thread> compRoutine;
	for (int i = 0; i < (int)mSysInfo.dwNumberOfProcessors; ++i) {
		compRoutine.push_back(std::thread(ProcessLoginPacket, pIOCP));
	}

	SocketAddress sa(INADDR_ANY, 9001);

	std::shared_ptr<TCPSocket> pSock = SocketUtil::CreateTCPSocket();
	pSock->Bind(sa);
	pSock->Listen(15);

	std::thread acceptThr(AcceptThread, pSock, pIOCP);

	for (int i = 0; i < (int)mSysInfo.dwNumberOfProcessors; ++i) {
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

		pClntSock->Receive();
	}
}

void NetworkManager::ProcessLoginPacket(std::shared_ptr<IOCP> pIOCP) {
	LPIOCP_KEY_DATA pKeyData;
	LPIO_DATA ioData;

	while (1) {
		pIOCP->GetCompletion(pKeyData, ioData);
		if (ioData->rwMode == MODE_READ) {
			InputBitStream ibs((uint8_t*)ioData->buffer, 8 * 40);

			char id[20], pw[20];
			ibs.ReadBytes(id, 20);
			ibs.ReadBytes(pw, 20);

			LOG(id);
			LOG(pw);

			// TODO: 인증 후, client proxy 추가 (db 연동)
		}	
	}
}
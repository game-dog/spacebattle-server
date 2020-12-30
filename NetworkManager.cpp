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

			std::string inID(20, 0), inPW(20, 0);
			ibs.ReadBytes(&inID[0], 20);
			ibs.ReadBytes(&inPW[0], 20);

			//LOG(inID.c_str());
			//LOG(inPW.c_str());

			bool result = Account::VerifyAccount(inID.c_str(), inPW.c_str());
			if (result == TRUE) {
				// TODO: 서버 접속 성공 패킷 전달
			}
			else {
				// TODO: 서버 접속 실패 패킷 전달
			}

			// TODO: 소켓 연결 종료
		}
	}
}
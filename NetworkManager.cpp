#include "NetworkManager.h"
#include "IOCP.h"

void NetworkManager::Init() {
	GetSystemInfo(&mSysInfo);
}

void NetworkManager::OpenSockets() {
	// 로그인 소켓 개방
	std::vector<std::thread> workers;

	std::thread loginAcceptThr;
	OpenLoginSocket(workers, loginAcceptThr);

	std::thread infoAcceptThr;
	OpenInfoSocket(workers, infoAcceptThr);

	std::thread gameAcceptThr;
	OpenGameSocket(workers, gameAcceptThr);

	// TODO: 게임 소켓 개방
	for (int i = 0; i < (int)mSysInfo.dwNumberOfProcessors; ++i) {
		workers[i].join();
	}
	loginAcceptThr.join();
}

void NetworkManager::OpenLoginSocket(std::vector<std::thread>& workers, std::thread& acceptThr) {
	std::shared_ptr<IOCP> pIOCP = SocketUtil::CreateIOCP();

	for (int i = 0; i < (int)mSysInfo.dwNumberOfProcessors; ++i) {
		workers.push_back(std::thread(ProcessLoginPacket, pIOCP, std::ref(*this)));
	}

	SocketAddress loginSa(INADDR_ANY, 9001);

	std::shared_ptr<TCPSocket> pLoginSock = SocketUtil::CreateTCPSocket();
	pLoginSock->Bind(loginSa);
	pLoginSock->Listen(15);

	acceptThr = std::thread(AcceptThread, pLoginSock, pIOCP);
}

void NetworkManager::OpenInfoSocket(std::vector<std::thread>& workers, std::thread& acceptThr) {

}

void NetworkManager::OpenGameSocket(std::vector<std::thread>& workers, std::thread& acceptThr) {

}

void NetworkManager::AcceptThread(std::shared_ptr<TCPSocket> pSock, std::shared_ptr<IOCP> pIOCP) {
	while (1) {
		SocketAddress clntAddr;
		std::shared_ptr<TCPSocket> pClntSock(pSock->Accept(clntAddr));

		pIOCP->ConnectSockToIOCP(pClntSock, clntAddr);
		pClntSock->Receive();
	}
}

void NetworkManager::ProcessLoginPacket(std::shared_ptr<IOCP> pIOCP, NetworkManager& nm) {
	LPIOCP_KEY_DATA pKeyData;
	LPIO_DATA ioData;

	while (1) {
		// TODO: IOCP와 NetworkManager 클래스를 이어주도록? ReceivedPacket queue 사용?
		int32_t byteTrans = pIOCP->GetCompletion(pKeyData, ioData);
		if (ioData->rwMode == MODE_READ) {
			uint8_t header = 0;
			std::string inID(20, 0), inPW(20, 0);
			nm.RecvLoginInfo(ioData, &header, inID, inPW);

			LOG(inID.c_str());
			LOG(inPW.c_str());

			bool isVerified = Account::VerifyAccount(inID.c_str(), inPW.c_str());
			bool isLoggedin = nm.GetClientProxy(inID.c_str()) == nullptr;
			if (isVerified == TRUE && isLoggedin == TRUE) {
				LOG("NEW CLIENT LOGGED IN");
				std::shared_ptr<ClientProxy> newClientProxy = std::make_shared<ClientProxy>(pKeyData->pClntSock, pKeyData->clntAddr, inID);

				// TODO: 클라이언트의 추가적인 정보 초기화 (전적 등)
				nm.AddClientProxy(inID.c_str(), newClientProxy);
				nm.SendLoginSuccessPacket(pKeyData->pClntSock);
			}
			else {
				nm.SendLoginFailedPacket(pKeyData->pClntSock);
			}
		}
		else {
			LOG("MODE_SEND");
			pKeyData->pClntSock->~TCPSocket();
			free(pKeyData);
		}
	}
}
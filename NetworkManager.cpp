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
	//OpenGameSocket(workers, gameAcceptThr);

	// TODO: 게임 소켓 개방
	for (int i = 0; i < (int)mSysInfo.dwNumberOfProcessors; ++i) {
		workers[i].join();
	}
	loginAcceptThr.join();
	infoAcceptThr.join();
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
	std::shared_ptr<IOCP> pIOCP = SocketUtil::CreateIOCP();

	for (int i = 0; i < (int)mSysInfo.dwNumberOfProcessors; ++i) {
		workers.push_back(std::thread(ProcessInfoPacket, pIOCP, std::ref(*this)));
	}

	SocketAddress infoSa(INADDR_ANY, 9002);

	std::shared_ptr<TCPSocket> pInfoSock = SocketUtil::CreateTCPSocket();
	pInfoSock->Bind(infoSa);
	pInfoSock->Listen(15);

	acceptThr = std::thread(AcceptThread, pInfoSock, pIOCP);
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

			bool isVerified = Account::VerifyAccount(inID.c_str(), inPW.c_str());
			bool isLoggedin = nm.GetClientProxy(inID.c_str()) == nullptr;
			if (isVerified == TRUE && isLoggedin == TRUE) {
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
			pKeyData->pClntSock->~TCPSocket();
			free(pKeyData);
		}
	}
}

void NetworkManager::ProcessInfoPacket(std::shared_ptr<IOCP> pIOCP, NetworkManager& nm) {
	LPIOCP_KEY_DATA pKeyData;
	LPIO_DATA ioData;

	while (1) {
		int32_t byteTrans = pIOCP->GetCompletion(pKeyData, ioData);
		if (ioData->rwMode == MODE_READ) {
			if (nm.IsVerifiedUser(pKeyData->clntAddr)) {
				uint8_t header = 0;

				InputBitStream ibs((uint8_t*)ioData->buffer, 4);
				ibs.ReadBits(reinterpret_cast<void*>(header), 4);

				switch (header) {
				case CHAT_REQ:
					1;
					break;
				case USER_REQ:
					1;
					break;
				case ROOM_REQ:
					1;
					break;
				default:
					LOG("Unknown packet type received from %s", "0");
					break;
				}
			}
			else {
				// TODO: 인증이 되지 않은 사용자에게 받은 패킷은 무시 후, 소켓 연결 종료
				LOG("Bad incoming packet from unknown client at socket %s", "0");
			}
		}
	}
}
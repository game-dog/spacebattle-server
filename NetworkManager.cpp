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

	acceptThr = std::thread(LoginAcceptThread, pLoginSock, pIOCP);
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

	acceptThr = std::thread(InfoAcceptThread, pInfoSock, pIOCP, std::ref(*this));
}

void NetworkManager::LoginAcceptThread(std::shared_ptr<TCPSocket> pSock, std::shared_ptr<IOCP> pIOCP) {
	while (1) {
		SocketAddress clntAddr;
		std::shared_ptr<TCPSocket> pClntSock(pSock->Accept(clntAddr));

		pIOCP->ConnectSockToIOCP(pClntSock, clntAddr);
		pClntSock->Receive();
	}
}

void NetworkManager::InfoAcceptThread(std::shared_ptr<TCPSocket> pSock, std::shared_ptr<IOCP> pIOCP, NetworkManager &nm) {
	while (1) {
		SocketAddress clntAddr;
		std::shared_ptr<TCPSocket> pClntSock(pSock->Accept(clntAddr));

		if (!nm.IsVerifiedUser(clntAddr)) {		
			LOG("Bad incoming packet from unverified client at socket %s", "0");
			continue;
		}

		nm.SetClientInfoSocket(clntAddr, pClntSock);
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
				std::shared_ptr<ClientProxy> newClientProxy = std::make_shared<ClientProxy>(pKeyData->clntAddr, inID);

				// TODO: 클라이언트의 추가적인 정보 초기화 (전적 등)
				nm.AddClientProxy(inID.c_str(), pKeyData->clntAddr, newClientProxy);
				nm.SendLoginSuccessPacket(pKeyData->pClntSock);
				LOG("Server Welcoming, new client %s as player %s",
					newClientProxy->GetSocketAddress().ToString().c_str(),
					newClientProxy->GetClientId().c_str());
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
			nm.HandlePacket(ioData->buffer, pKeyData->clntAddr);
		}
	}
}

void NetworkManager::HandlePacket(const char* buffer, SocketAddress addr) {
	std::shared_ptr<ClientProxy> clientProxy = GetClientProxy(addr);
	uint8_t header = 0;

	InputBitStream ibs((uint8_t*)buffer, 4);
	ibs.ReadBits(reinterpret_cast<void*>(header), 4);

	switch (header) {
	case CHAT_REQ:
		SendChatPacket(ibs, addr);
		break;
	case USER_REQ:
		// SendUserInfoPacket(ibs);
		break;
	case ROOM_REQ:
		// SendRoomInfoPacket(ibs);
		break;
	default:
		LOG("Unknown packet type received from %s", clientProxy->GetSocketAddress().ToString().c_str());
		break;
	}
}

void NetworkManager::SendChatPacket(InputBitStream& ibs, const SocketAddress& addr) {
	uint8_t header = 0, sz = 0;
	ibs.ReadBits(reinterpret_cast<void*>(header), 1);
	ibs.ReadBits(reinterpret_cast<void*>(sz), 8);

	uint8_t* content = new uint8_t[sz];
	ibs.ReadBytes(reinterpret_cast<void*>(content), sz);

	std::string id = mAddrToClientProxyMap[addr]->GetClientId();
	OutputBitStream obs;

	switch (header) {
	case LOBBY:
		obs.WriteBits(static_cast<uint8_t>(CHAT_RES), 4);
		obs.WriteBits(static_cast<uint8_t>(0), 1);
		obs.WriteBits(reinterpret_cast<void*>(const_cast<char*>(id.c_str())), 8 * 20);
		obs.WriteBits(sz, 8);
		obs.WriteBits(reinterpret_cast<void*>(content), 8 * sz);
		delete[] content;
		for (auto& c : mAddrToClientProxyMap) {
			if (c.second->GetLocation() == LOCATION_LOBBY) {
				const std::shared_ptr<TCPSocket> sock = c.second->GetInfoSocket();
				sock->Send(reinterpret_cast<const void*>(obs.GetBufferPtr()), obs.GetByteLength());
			}
		}
		break;
	case ROOM:
		// TODO: 채팅 패킷을 보낸 유저가 소속된 방이 어디인지 확인
		// TODO: 확인된 방 번호에 속해있는 유저에게 채팅 패킷 전달
		break;
	}
}
#include "NetworkManager.h"
#include "IOCP.h"

void NetworkManager::Init() {
	GetSystemInfo(&mSysInfo);
}

void NetworkManager::OpenSockets() {
	std::shared_ptr<IOCP> pIOCP = SocketUtil::CreateIOCP();

	std::vector<std::thread> compRoutine;
	for (int i = 0; i < (int)mSysInfo.dwNumberOfProcessors; ++i) {
		compRoutine.push_back(std::thread(ProcessLoginPacket, pIOCP, std::ref(*this)));
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

	// TODO: ä�� ���� ����
	// TODO: �÷��� ���� ����
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
		// TODO: IOCP�� NetworkManager Ŭ������ �̾��ֵ���? ReceivedPacket queue ���?
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

				// TODO: Ŭ���̾�Ʈ�� �߰����� ���� �ʱ�ȭ (���� ��)
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
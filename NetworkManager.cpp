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
			if (byteTrans == 0) {
				LOG("CLIENT DISCONNECTED");
				free(pKeyData);
				continue;
			}
			InputBitStream ibs((uint8_t*)ioData->buffer, 4 + 8 * 40);

			uint8_t header;
			std::string inID(20, 0), inPW(20, 0);
			ibs.ReadBits((void*)&header, 4);
			ibs.ReadBytes(&inID[0], 20);
			ibs.ReadBytes(&inPW[0], 20);

			LOG(inID.c_str());
			LOG(inPW.c_str());

			bool result = Account::VerifyAccount(inID.c_str(), inPW.c_str());
			if (result == TRUE) {
				if (nm.GetClientProxy(inID.c_str()) == nullptr) {
					std::shared_ptr<ClientProxy> newClientProxy = std::make_shared<ClientProxy>(pKeyData->clntAddr, inID);

					// TODO: Ŭ���̾�Ʈ�� �߰����� ���� �ʱ�ȭ (���� ��)

					nm.AddClientProxy(inID.c_str(), newClientProxy);

					// TODO: ���� ���� ���� ��Ŷ ����
				}
				else {

				}
			}
			else {
				// TODO: ���� ���� ���� ��Ŷ ����
			}

			pKeyData->pClntSock->Receive();
		}
	}
}
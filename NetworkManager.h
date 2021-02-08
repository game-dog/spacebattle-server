#ifndef __NETWORK_MANAGER_H__
#define __NETWORK_MANAGER_H__

#include <WinSock2.h>
#include <vector>
#include <string>
#include <thread>
#include <unordered_map>

#include "TCPSocket.h"
#include "SocketUtil.h"
#include "SocketAddress.h"
#include "BitStream.h"
#include "Account.h"
#include "ClientProxy.h"
#include "Logger.h"

class IOCP;

enum {
	CHAT_REQ,
	USER_REQ,
	ROOM_REQ
};

enum {
	CHAT_RES,
	USER_RES,
	ROOM_RES
};

class NetworkManager {
public:
	void Init();

	void OpenSockets();
	bool IsVerifiedUser(SocketAddress clntAddr) {
		auto it = mAddrToClientProxyMap.find(clntAddr);
		if (it == mAddrToClientProxyMap.end()) return false;
		return true;
	}

private:
	using IdToClientProxyMap = std::unordered_map<std::string, std::shared_ptr<ClientProxy>>;
	using AddrToClientProxyMap = std::unordered_map<SocketAddress, std::shared_ptr<ClientProxy>>;

	SYSTEM_INFO mSysInfo;
	IdToClientProxyMap mIdToClientProxyMap;
	AddrToClientProxyMap mAddrToClientProxyMap;

	std::shared_ptr<ClientProxy> GetClientProxy(const char* id) {
		auto it = mIdToClientProxyMap.find(id);
		if (it != mIdToClientProxyMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	void OpenLoginSocket(std::vector<std::thread>& workers, std::thread& acceptThr);
	void OpenInfoSocket(std::vector<std::thread>& workers, std::thread& acceptThr);
	//void OpenGameSocket(std::vector<std::thread>& workers, std::thread& acceptThr);

	void AddClientProxy(std::string id, std::shared_ptr<ClientProxy> pCP) { mIdToClientProxyMap[id] = pCP; }
	void RemoveClientProxy(std::string id) { mIdToClientProxyMap.erase(id); }

	void RecvLoginInfo(LPIO_DATA ioData, uint8_t* header, std::string& inID, std::string& inPW) {
		InputBitStream ibs((uint8_t*)ioData->buffer, 4 + 8 * 40);
		ibs.ReadBits(reinterpret_cast<void*>(header), 4);
		ibs.ReadBytes(&inID[0], 20);
		ibs.ReadBytes(&inPW[0], 20);
	}

	void SendLoginSuccessPacket(std::shared_ptr<TCPSocket> pSock) {
		OutputBitStream obs;
		obs.WriteBits(static_cast<uint8_t>(0), 4);
		obs.WriteBits(1, 1);
		pSock->Send(reinterpret_cast<const void*>(obs.GetBufferPtr()), obs.GetByteLength());
	}

	void SendLoginFailedPacket(std::shared_ptr<TCPSocket> pSock) {
		OutputBitStream obs;
		obs.WriteBits(static_cast<uint8_t>(0), 4);
		obs.WriteBits(static_cast<uint8_t>(0), 1);
		pSock->Send(reinterpret_cast<const void*>(obs.GetBufferPtr()), obs.GetByteLength());
	}

	static void AcceptThread(std::shared_ptr<TCPSocket> pSock, std::shared_ptr<IOCP> pIOCP);
	static void ProcessLoginPacket(std::shared_ptr<IOCP> pIOCP, NetworkManager& nm);
	static void ProcessInfoPacket(std::shared_ptr<IOCP> pIOCP, NetworkManager& nm);
};

#endif
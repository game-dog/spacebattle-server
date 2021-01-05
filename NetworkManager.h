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

class NetworkManager {
public:
	void Init();

	void OpenSockets();

private:
	typedef std::unordered_map<std::string, std::shared_ptr<ClientProxy>> IdToClientProxyMap;

	SYSTEM_INFO mSysInfo;
	IdToClientProxyMap mIdToClientProxyMap;

	std::shared_ptr<ClientProxy> GetClientProxy(const char* id) {
		auto it = mIdToClientProxyMap.find(id);
		if (it != mIdToClientProxyMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	void AddClientProxy(std::string id, std::shared_ptr<ClientProxy> pCP) { mIdToClientProxyMap[id] = pCP; }

	static void AcceptThread(std::shared_ptr<TCPSocket> pSock, std::shared_ptr<IOCP> pIOCP);
	static void ProcessLoginPacket(std::shared_ptr<IOCP> pIOCP, NetworkManager& nm);
};

#endif
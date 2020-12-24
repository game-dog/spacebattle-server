#ifndef __NETWORK_MANAGER_H__
#define __NETWORK_MANAGER_H__

#include <WinSock2.h>
#include <vector>
#include <thread>

#include "TCPSocket.h"
#include "SocketUtil.h"
#include "SocketAddress.h"
#include "BitStream.h"
#include "Logger.h"

class IOCP;

class NetworkManager {
public:
	void Init();

	void OpenSockets();

private:
	SYSTEM_INFO mSysInfo;

	static void AcceptThread(std::shared_ptr<TCPSocket> pSock, std::shared_ptr<IOCP> pIOCP);
	static void ProcessLoginPacket(std::shared_ptr<IOCP> pIOCP);
};

#endif
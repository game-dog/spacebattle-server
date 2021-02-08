#ifndef __CLIENT_PROXY_H__
#define __CLIENT_PROXY_H__

#include <string>

#include "TCPSocket.h"
#include "SocketAddress.h"

class ClientProxy {
public:
	ClientProxy(const std::shared_ptr<TCPSocket> inSock, const SocketAddress& inSocketAddress, const std::string& inID) : 
		mSock(inSock), mSockAddr(inSocketAddress), mId(inID) {}

	const SocketAddress& GetSocketAddress()	const { return mSockAddr; }
	const std::string& GetClientId() { return mId; }

private:
	std::shared_ptr<TCPSocket> mSock;
	SocketAddress mSockAddr;
	std::string mId;
};

#endif
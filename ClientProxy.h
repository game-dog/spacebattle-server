#ifndef __CLIENT_PROXY_H__
#define __CLIENT_PROXY_H__

#include <string>

#include "TCPSocket.h"
#include "SocketAddress.h"

class ClientProxy {
public:
	ClientProxy(const std::shared_ptr<TCPSocket> inSock, const SocketAddress& inSocketAddress, const std::string& inID) : 
		sock(inSock), sockAddr(inSocketAddress), id(inID) {}

private:
	std::shared_ptr<TCPSocket> sock;
	SocketAddress sockAddr;
	std::string id;
};

#endif
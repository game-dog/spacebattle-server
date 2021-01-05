#ifndef __CLIENT_PROXY_H__
#define __CLIENT_PROXY_H__

#include <string>

#include "SocketAddress.h"

class ClientProxy {
public:
	ClientProxy(const SocketAddress& inSocketAddress, const std::string& inID) : sockAddr(inSocketAddress), id(inID) {}

private:
	SocketAddress sockAddr;
	std::string id;
};

#endif
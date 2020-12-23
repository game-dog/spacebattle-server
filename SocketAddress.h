#ifndef __SOCKET_ADDRESS_H__
#define __SOCKET_ADDRESS_H__

#include <cstdint>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>

class SocketAddress {
public:
	SocketAddress() { }

	SocketAddress(std::string inAddress, uint16_t inPort) {
		GetAsSockAddrIn()->sin_family = AF_INET;
		inet_pton(AF_INET, inAddress.c_str(), &(GetAsSockAddrIn()->sin_addr.S_un.S_addr));
		GetAsSockAddrIn()->sin_port = htons(inPort);
	}

	SocketAddress(uint32_t inAddress, uint16_t inPort) {
		GetAsSockAddrIn()->sin_family = AF_INET;
		GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(inAddress);
		GetAsSockAddrIn()->sin_port = htons(inPort);
	}

	SocketAddress(const SOCKADDR& inSockAddr) {
		memcpy(&mSockAddr, &inSockAddr, sizeof(SOCKADDR));
	}

	size_t GetSize() const { return sizeof(SOCKADDR); }

private:
	friend class TCPSocket;

	SOCKADDR mSockAddr;

	SOCKADDR_IN* GetAsSockAddrIn() {
		return reinterpret_cast<SOCKADDR_IN*>(&mSockAddr);
	}
};

#endif
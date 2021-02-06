#ifndef __SOCKET_ADDRESS_H__
#define __SOCKET_ADDRESS_H__

#include <cstdint>
#include <functional>
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

	bool operator==(const SocketAddress& s) const {
		return (mSockAddr.sa_family == AF_INET &&
			GetAsSockAddrIn()->sin_port == s.GetAsSockAddrIn()->sin_port) &&
			(GetIP4Ref() == s.GetIP4Ref());
	}

	size_t GetHash() const {
		return (GetIP4Ref()) |
			((static_cast<uint32_t>(GetAsSockAddrIn()->sin_port)) << 13) |
			mSockAddr.sa_family;
	}

	std::string ToString() const;

private:
	friend class TCPSocket;
	friend struct std::hash<SocketAddress>;

	SOCKADDR mSockAddr;

	uint32_t& GetIP4Ref() { return *reinterpret_cast<uint32_t*>(&GetAsSockAddrIn()->sin_addr.S_un.S_addr); }
	const uint32_t& GetIP4Ref() const { return *reinterpret_cast<const uint32_t*>(&GetAsSockAddrIn()->sin_addr.S_un.S_addr); }

	SOCKADDR_IN* GetAsSockAddrIn() { return reinterpret_cast<SOCKADDR_IN*>(&mSockAddr); }
	const SOCKADDR_IN* GetAsSockAddrIn() const { return reinterpret_cast<const SOCKADDR_IN*>(&mSockAddr); }
};

namespace std {
	template<>
	struct std::hash<SocketAddress> {
		size_t operator()(const SocketAddress& s) const {
			return s.GetHash();
		}
	};
}

#endif
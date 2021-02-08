#include "SocketAddress.h"

std::string SocketAddress::ToString() const {
	const SOCKADDR_IN* s = GetAsSockAddrIn();
	char destinationBuffer[128];
	inet_ntop(s->sin_family, const_cast<in_addr*>(&s->sin_addr), destinationBuffer, sizeof(destinationBuffer));
	return Logger::Sprintf("%s:%d", destinationBuffer, ntohs(s->sin_port));
}
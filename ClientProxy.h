#ifndef __CLIENT_PROXY_H__
#define __CLIENT_PROXY_H__

#include <string>

#include "TCPSocket.h"
#include "SocketAddress.h"

enum {
	LOCATION_LOBBY,
	LOCATION_ROOM
};

class ClientProxy {
public:
	ClientProxy(const SocketAddress& inSocketAddress, const std::string& inID) : 
		mLocation(LOCATION_LOBBY), mRoomNumber(0), mSockAddr(inSocketAddress), mId(inID) {}

	int GetLocation() const { return mLocation; }
	int GetRoomNumber() const {
		if (mLocation != LOCATION_LOBBY) return -1;
		return mRoomNumber;
	}
	const std::shared_ptr<TCPSocket>& GetInfoSocket() const { return mInfoSock; }
	const SocketAddress& GetSocketAddress()	const { return mSockAddr; }
	const std::string& GetClientId() { return mId; }

	void SetInfoSocket(std::shared_ptr<TCPSocket> sock) { mInfoSock = sock; }
	void SetGameSocket(std::shared_ptr<TCPSocket> sock) { mGameSock = sock; }

	void EnterRoom(int roomNumber) { mLocation = LOCATION_ROOM; mRoomNumber = roomNumber; }
	void LeaveRoom() { mLocation = LOCATION_LOBBY; }

private:
	int mLocation;
	int mRoomNumber;

	std::shared_ptr<TCPSocket> mInfoSock;
	std::shared_ptr<TCPSocket> mGameSock;
	SocketAddress mSockAddr;
	std::string mId;
};

#endif
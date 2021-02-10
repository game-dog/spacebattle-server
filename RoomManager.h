#ifndef __ROOM_MANAGER_H__
#define __ROOM_MANAGER_H__

#include <unordered_map>

class RoomManager {
private:
	class Room {
	public:
		Room() : mStatus(false) {}
		std::string GetOtherUser(std::string id) {
			if (id == user1) return user2;
			else return user1;
		}

	private:
		std::string user1;
		std::string user2;
		bool mStatus;
		int mRoomNumber;
	};

public:
	static RoomManager* GetInstance();
	static void DestroyInstance();

	int CreateRoom();
	void CloseRoom();

private:
	RoomManager() : mNext(0) {};
	~RoomManager() {};

	static RoomManager* m_pInst;

	int mNext;
	std::unordered_map<int, Room> mRoomMap;
};

#endif
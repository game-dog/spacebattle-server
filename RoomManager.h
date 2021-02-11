#ifndef __ROOM_MANAGER_H__
#define __ROOM_MANAGER_H__

#include <unordered_map>

class RoomManager {
private:
	class Room {
	public:
		Room(int mRoomNumber, std::string owner) :
			owner(owner),
			participant(""),
			mStatus(false),
			mRoomNumber(mRoomNumber) {}

		std::string GetOpponentId(std::string id) const {
			if (id == owner) return participant;
			else return owner;
		}
		
		void SetParticipant(std::string id) { participant = id; }

	private:
		std::string owner, participant;
		bool mStatus;
		int mRoomNumber;
	};

public:
	static RoomManager* GetInstance();
	static void DestroyInstance();

	int CreateRoom(std::string owner);
	void CloseRoom();

	const Room& GetRoomInstance(int number) { return mRoomMap[number]; }

private:
	RoomManager() : mNext(0) {};
	~RoomManager() {};

	static RoomManager* m_pInst;

	int mNext;
	std::unordered_map<int, Room> mRoomMap;
};

#endif
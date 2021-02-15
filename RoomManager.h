#ifndef __ROOM_MANAGER_H__
#define __ROOM_MANAGER_H__

#include <unordered_map>

class RoomManager {
private:
	class Room {
	public:
		Room() : owner(""), participant(""), mStatus(false), mRoomNumber(0) {}
		Room(int mRoomNumber, std::string owner) :
			owner(owner),
			participant(""),
			mStatus(false),
			mRoomNumber(mRoomNumber) {}

		std::string owner, participant;
		bool mStatus;
		int mRoomNumber;
	};

public:
	static RoomManager* GetInstance();
	static void DestroyInstance();

	int CreateRoom(std::string owner);
	void CloseRoom(int roomNumber);

	int GetRoomCount() const { return mNext; }
	std::vector<int> GetRoomNumbers() const;
	std::string GetOwnerId(int roomNumber) { return mRoomMap[roomNumber].owner; }
	std::string GetOpponentId(int roomNumber, std::string id);
	bool GetRoomStatus(int roomNumber) { return mRoomMap[roomNumber].mStatus; }

	void SetParticipantId(int roomNumber, std::string id) { mRoomMap[roomNumber].participant = id; }
	void DelegateOwnership(int roomNumber);

	//const Room& GetRoomInstance(int number) { return mRoomMap[number]; }
	//const std::unordered_map<int, Room>& GetRoomMap() { return mRoomMap; }

private:
	RoomManager() : mNext(0) {};
	~RoomManager() {};

	static RoomManager* m_pInst;

	int mNext;
	std::unordered_map<int, Room> mRoomMap;
};

#endif
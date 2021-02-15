#include "RoomManager.h"

RoomManager* RoomManager::m_pInst = nullptr;

RoomManager* RoomManager::GetInstance() {
	if (!m_pInst) m_pInst = new RoomManager;
	return m_pInst;
}

void RoomManager::DestroyInstance() {
	if (!m_pInst) return;
	delete m_pInst;
	m_pInst = nullptr;
}

int RoomManager::CreateRoom(std::string owner) {
	mRoomMap[++mNext] = Room(mNext, owner);
	return mNext;
}

void RoomManager::CloseRoom(int roomNumber) {
	mRoomMap.erase(roomNumber);
}

std::vector<int> RoomManager::GetRoomNumbers() const {
	std::vector<int> numbers;
	for (auto& room : mRoomMap) numbers.push_back(room.first);
	return numbers;
}

std::string RoomManager::GetOpponentId(int roomNumber, std::string id) {
	const Room& room = mRoomMap[roomNumber];
	if (room.owner == id) return room.participant;
	else return room.owner;
}

void RoomManager::DelegateOwnership(int roomNumber) {
	mRoomMap[roomNumber].owner = mRoomMap[roomNumber].participant;
	mRoomMap[roomNumber].participant = "";
}
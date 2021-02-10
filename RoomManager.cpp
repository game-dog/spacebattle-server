#include "RoomManager.h"

RoomManager* RoomManager::m_pInst = nullptr;

RoomManager* RoomManager::GetInstance() {
	if (!m_pInst) {
		m_pInst = new RoomManager;
	}
	return m_pInst;
}

void RoomManager::DestroyInstance() {
	if (!m_pInst) {
		return;
	}
	delete m_pInst;
	m_pInst = nullptr;
}

int RoomManager::CreateRoom() {
	mRoomMap[++mNext] = Room();
	return mNext;
}

void RoomManager::CloseRoom() {}
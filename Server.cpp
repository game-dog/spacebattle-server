#include "Server.h"
#include "NetworkManager.h"
#include "Database.h"
#include "Account.h"

int Server::Run() {
	Database::Init();

	Account::GetAccountData();

	// TODO: 네트워크 매니저 초기화 (로그인, 채팅, 플레이 소켓 생성)
	NetworkManager nm;

	nm.Init();
	nm.OpenSockets();

	// TODO: 클래스 정보 초기화

	return 0;
}
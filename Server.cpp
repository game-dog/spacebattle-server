#include "Server.h"
#include "NetworkManager.h"
#include "Database.h"
#include "Account.h"

int Server::Run() {
	Database::Init();

	Account::GetAccountData();

	// TODO: ��Ʈ��ũ �Ŵ��� �ʱ�ȭ (�α���, ä��, �÷��� ���� ����)
	NetworkManager nm;

	nm.Init();
	nm.OpenSockets();

	// TODO: Ŭ���� ���� �ʱ�ȭ

	return 0;
}
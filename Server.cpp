#include "Server.h"
#include "NetworkManager.h"

int Server::Run() {
	// TODO: ��Ʈ��ũ �Ŵ��� �ʱ�ȭ (�α���, ä��, �÷��� ���� ����)
	NetworkManager nm;

	nm.Init();
	nm.OpenSockets();

	// TODO: Ŭ���� ���� �ʱ�ȭ

	return 0;
}
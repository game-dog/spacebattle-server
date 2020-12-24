#include "SocketUtil.h"
#include "Server.h"
#include "BitStream.h"
#include "Database.h"

int main(int argc, char* argv[]) {
	Database::Connect();

	if (SocketUtil::Init()) {
		return Server::Run();
	}
	else { return -1; }
}
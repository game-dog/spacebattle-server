#include "SocketUtil.h"
#include "Server.h"

int main(int argc, char* argv[]) {
	if (SocketUtil::Init()) {
		return Server::Run();
	}
	else { return -1; }
}
#include "Account.h"

const char* Account::selectQuery = "SELECT * FROM account";
std::unordered_map<std::string, std::string> Account::mIDtoPwDigest = std::unordered_map<std::string, std::string>();

void Account::GetAccountData() {
	std::vector<std::vector<std::string>> dat = Database::Query((const char*)selectQuery);
	for (auto& entry : dat) {
		mIDtoPwDigest[entry[1]] = entry[2];
	}
}
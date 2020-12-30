#include "Account.h"

const char* Account::selectQuery = "SELECT * FROM account";
std::unordered_map<std::string, std::string> Account::mIDtoPwDigest = std::unordered_map<std::string, std::string>();

void Account::GetAccountData() {
	std::vector<std::vector<std::string>> dat = Database::Query((const char*)selectQuery);
	for (auto& entry : dat) {
		mIDtoPwDigest[entry[1]] = entry[2];
	}
}

bool Account::VerifyAccount(const char* id, const char* pw) {
	std::string hash = Crypto::md5(std::string(pw));
	std::string pwDigest = Account::GetPasswd(id);
	if (pwDigest == "") {
		return FALSE;
	}
	else {
		if (hash == pwDigest) return TRUE;
		else return FALSE;
	}
}
#include "Account.h"

const char* Account::selectQuery = "SELECT * FROM account";
std::unordered_map<std::string, std::shared_ptr<Account::UserData>> Account::mIDtoUserData = std::unordered_map<std::string, std::shared_ptr<Account::UserData>>();

void Account::GetAccountData() {
	std::vector<std::vector<std::string>> dat = Database::Query((const char*)selectQuery);
	for (auto& entry : dat) {
		mIDtoUserData[entry[1]] = std::make_shared<UserData>();
		mIDtoUserData[entry[1]]->pwDigest = entry[2];
		mIDtoUserData[entry[1]]->win = stoi(entry[3]);
		mIDtoUserData[entry[1]]->lose = stoi(entry[4]);
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
#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

#include "Database.h"
#include "Crypto.h"

class Account {
public:
	static void GetAccountData();
	static std::string GetPasswd(const char* id) { return mIDtoUserData[id]->pwDigest; }
	static bool VerifyAccount(const char* id, const char* pw);

	class UserData {
	public:
		std::string pwDigest;
	};

private:
	static const char* selectQuery;
	static std::unordered_map<std::string, std::shared_ptr<UserData>> mIDtoUserData;
};

#endif
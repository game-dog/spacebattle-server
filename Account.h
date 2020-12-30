#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__

#include <vector>
#include <string>
#include <unordered_map>

#include "Database.h"
#include "Crypto.h"

class Account {
public:
	static void GetAccountData();
	static std::string GetPasswd(const char* id) { return mIDtoPwDigest[id]; }
	static bool VerifyAccount(const char* id, const char* pw);

private:
	static const char* selectQuery;
	static std::unordered_map<std::string, std::string> mIDtoPwDigest;
};

#endif
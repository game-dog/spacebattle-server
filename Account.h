#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__

#include <vector>
#include <string>
#include <unordered_map>

#include "Database.h"

class Account {
public:
	static void GetAccountData();
	static std::string GetPasswd(const char* id) { return mIDtoPwDigest[id]; }

private:
	static const char* selectQuery;
	static std::unordered_map<std::string, std::string> mIDtoPwDigest;
};

#endif
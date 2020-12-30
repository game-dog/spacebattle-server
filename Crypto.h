#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#include <openssl/md5.h>
#include <string>

class Crypto {
public:
	static std::string md5(std::string plain);
};

#endif
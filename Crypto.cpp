#include "Crypto.h"

std::string Crypto::md5(std::string plain) {
	uint8_t* hash = new uint8_t[16];
	std::string ret(32, 0);

	MD5((const unsigned char*)&plain[0], plain.length(), (unsigned char*)&hash[0]);
	for (int i = 0, j = 0; i < 16; ++i, j += 2) {
		ret[j] = (hash[i] >> 4) & 0x0f;
		ret[j + 1] = hash[i] & 0x0f;

		if (ret[j] >= 10) ret[j] = 'a' + ret[j] - 10;
		else ret[j] += '0';
		if (ret[j + 1] >= 10) ret[j + 1] = 'a' + ret[j + 1] - 10;
		else ret[j + 1] += '0';
	}

	delete[] hash;
	return ret;
}
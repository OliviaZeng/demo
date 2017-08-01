#ifndef _CRYPTOTEST_H_
#define _CRYPTOTEST_H_

#include <stdint.h>

typedef enum {
	GENERAL = 0,
	ECB = 0x1,
	CBC = 0x2,
	CFB = 0x3,
	TRIPLE_ECB = 0x4,
	TRIPLE_CBC = 0x5,
}CRYPTO_MODE;

typedef struct cryptoInfo {
    uint32_t encodeLen;
    uint32_t decodeLen;
    unsigned char *encodebuf;
    unsigned char *decodebuf;
}CryptoInfo;


int DES_Init(void);
unsigned char *DES_Encrypt(const unsigned char *cleartext, unsigned int len);
unsigned char *DES_Decrypt(const unsigned char *ciphertext, unsigned int len);
#endif

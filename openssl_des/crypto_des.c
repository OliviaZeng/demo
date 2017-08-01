#include <string.h>
#include <openssl/des.h>
#include "crypto_des.h"

#define ENCODE_BUF_LEN 16384


static CryptoInfo *pCrytoInfo = NULL; 
CRYPTO_MODE mode = TRIPLE_CBC;
static char key[]  =  "12345678123456781234567812345678";
static unsigned char cbc_iv[8] = {'1', '2', '3', '4', '5', '6', '7', '8'};
static int key_len = 32;

int DES_Init(void)
{
    pCrytoInfo = (CryptoInfo *)malloc(sizeof(CryptoInfo));
    if (NULL == pCrytoInfo)
        return -1;

    pCrytoInfo->encodebuf = (unsigned char *)calloc(1, ENCODE_BUF_LEN);
    if (NULL == pCrytoInfo->encodebuf)
        return -1;

    pCrytoInfo->decodebuf = (unsigned char *)calloc(1, ENCODE_BUF_LEN);
    if (NULL == pCrytoInfo->decodebuf)
        return -1;

    pCrytoInfo->encodeLen = ENCODE_BUF_LEN;
    pCrytoInfo->decodeLen = ENCODE_BUF_LEN;

    return 0;
}

unsigned char *DES_Encrypt(const unsigned char *cleartext, unsigned int len)
{
    if (len > pCrytoInfo->encodeLen) {
        while (len > pCrytoInfo->encodeLen)
            pCrytoInfo->encodeLen <<= 1;

        unsigned char *p = (unsigned char *)realloc(pCrytoInfo->encodebuf, pCrytoInfo->encodeLen);
        if (!p)
            return p;
        pCrytoInfo->encodebuf = p;
    }

    switch (mode) {
        case GENERAL:
        case ECB:
        {
            DES_cblock keyEncrypt;
            memset(keyEncrypt, 0, 8);

            if (key_len <= 8)
                memcpy(keyEncrypt, key, key_len);
            else
                memcpy(keyEncrypt, key, 8);

            DES_key_schedule keySchedule;
            DES_set_key_unchecked(&keyEncrypt, &keySchedule);

            const_DES_cblock inputText;
            DES_cblock outputText;

            size_t i;
            for (i = 0; i < len / 8; i ++) {
                memcpy(inputText, cleartext + i * 8, 8);
                DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_ENCRYPT);
                memcpy(pCrytoInfo->encodebuf+i*8, outputText, 8);
            }

            if (len % 8 != 0) {
                int tmp1 = len / 8 * 8;
                int tmp2 = len - tmp1;
                memset(inputText, 0, 8);
                memcpy(inputText, cleartext + tmp1, tmp2);

                DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_ENCRYPT);
                memcpy(pCrytoInfo->encodebuf+i*8, outputText, 8);
            }
        }
        break;

        case CBC:
        {
            DES_cblock keyEncrypt, ivec;
            memset(keyEncrypt, 0, 8);

            if (key_len <= 8)
                memcpy(keyEncrypt, key, key_len);
            else
                memcpy(keyEncrypt, key, 8);

            DES_key_schedule keySchedule;
            DES_set_key_unchecked(&keyEncrypt, &keySchedule);
            memcpy(ivec, cbc_iv, sizeof(cbc_iv));
            DES_ncbc_encrypt((const unsigned char*)cleartext, pCrytoInfo->encodebuf, len+1, &keySchedule, &ivec, DES_ENCRYPT);

        }
        break;
        case CFB:
        {
            DES_cblock keyEncrypt, ivec;
            memset(keyEncrypt, 0, 8);

            if (key_len <= 8)
                memcpy(keyEncrypt, key, key_len);
            else
                memcpy(keyEncrypt, key, 8);

            DES_key_schedule keySchedule;
            DES_set_key_unchecked(&keyEncrypt, &keySchedule);
            memcpy(ivec, cbc_iv, sizeof(cbc_iv));
            DES_cfb_encrypt(cleartext, pCrytoInfo->encodebuf, 8, len, &keySchedule, &ivec, DES_ENCRYPT);

        }
        break;

        case TRIPLE_ECB:
        {
            DES_cblock ke1, ke2, ke3;
            memset(ke1, 0, 8);
            memset(ke2, 0, 8);
            memset(ke2, 0, 8);

            if (key_len >= 24) {
                memcpy(ke1, key, 8);
                memcpy(ke2, key + 8, 8);
                memcpy(ke3, key + 16, 8);
            } else if (key_len >= 16) {
                memcpy(ke1, key, 8);
                memcpy(ke2, key + 8, 8);
                memcpy(ke3, key + 16, key_len - 16);
            } else if (key_len >= 8) {
                memcpy(ke1, key, 8);
                memcpy(ke2, key + 8, key_len - 8);
                memcpy(ke3, key, 8);
            } else {
                memcpy(ke1, key, key_len);
                memcpy(ke2, key, key_len);
                memcpy(ke3, key, key_len);
            }

            DES_key_schedule ks1, ks2, ks3;
            DES_set_key_unchecked(&ke1, &ks1);
            DES_set_key_unchecked(&ke2, &ks2);
            DES_set_key_unchecked(&ke3, &ks3);

            const_DES_cblock inputText;
            DES_cblock outputText;

            size_t i;
            for (i = 0; i < len / 8; i ++) {
                memcpy(inputText, cleartext + i * 8, 8);
                DES_ecb3_encrypt(&inputText, &outputText, &ks1, &ks2, &ks3, DES_ENCRYPT);
                memcpy(pCrytoInfo->encodebuf+i*8, outputText, 8);
            }

            if (len % 8 != 0) {
                int tmp1 = len / 8 * 8;
                int tmp2 = len - tmp1;
                memset(inputText, 0, 8);
                memcpy(inputText, cleartext + tmp1, tmp2);

                DES_ecb3_encrypt(&inputText, &outputText, &ks1, &ks2, &ks3, DES_ENCRYPT);
                memcpy(pCrytoInfo->encodebuf+i*8, outputText, 8);
            }
        }
        break;

        case TRIPLE_CBC:
        {
            DES_cblock ke1, ke2, ke3, ivec;
            memset(ke1, 0, 8);
            memset(ke2, 0, 8);
            memset(ke2, 0, 8);

            if (key_len >= 24) {
                memcpy(ke1, key, 8);
                memcpy(ke2, key + 8, 8);
                memcpy(ke3, key + 16, 8);
            } else if (key_len >= 16) {
                memcpy(ke1, key, 8);
                memcpy(ke2, key + 8, 8);
                memcpy(ke3, key + 16, key_len - 16);
            } else if (key_len >= 8) {
                memcpy(ke1, key, 8);
                memcpy(ke2, key + 8, key_len - 8);
                memcpy(ke3, key, 8);
            } else {
                memcpy(ke1, key, key_len);
                memcpy(ke2, key, key_len);
                memcpy(ke3, key, key_len);
            }

            DES_key_schedule ks1, ks2, ks3;
            DES_set_key_unchecked(&ke1, &ks1);
            DES_set_key_unchecked(&ke2, &ks2);
            DES_set_key_unchecked(&ke3, &ks3);

            memcpy(ivec, cbc_iv, sizeof(cbc_iv));
            DES_ede3_cbc_encrypt((const unsigned char*)cleartext, pCrytoInfo->encodebuf, len+1, &ks1, &ks2, &ks3, &ivec, DES_ENCRYPT);
        }
        break;

        default:
            return NULL;

    }

    return pCrytoInfo->encodebuf;
}

unsigned char *DES_Decrypt(const unsigned char *ciphertext, unsigned int len)
{
    if (len > pCrytoInfo->decodeLen) {
        while (len > pCrytoInfo->decodeLen)
            pCrytoInfo->decodeLen <<= 1;

        unsigned char *p = (unsigned char *)realloc(pCrytoInfo->decodebuf, pCrytoInfo->decodeLen);
        if (!p)
            return p;
        pCrytoInfo->decodebuf = p;
    }

    switch (mode) {
        case GENERAL:
        case ECB:
        {
            DES_cblock keyEncrypt;
            memset(keyEncrypt, 0, 8);

            if (key_len <= 8)
                    memcpy(keyEncrypt, key, key_len);
            else
                    memcpy(keyEncrypt, key, 8);

            DES_key_schedule keySchedule;
            DES_set_key_unchecked(&keyEncrypt, &keySchedule);

            const_DES_cblock inputText;
            DES_cblock outputText;

            size_t i;
            for (i = 0; i < len / 8; i ++) {
                memcpy(inputText, ciphertext + i * 8, 8);
                DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_DECRYPT);
                memcpy(pCrytoInfo->decodebuf+i*8, outputText, 8);
            }

            if (len % 8 != 0) {
                int tmp1 = len / 8 * 8;
                int tmp2 = len - tmp1;
                memset(inputText, 0, 8);
                memcpy(inputText, ciphertext + tmp1, tmp2);

                DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_DECRYPT);
                memcpy(pCrytoInfo->decodebuf+i*8, outputText, 8);
            }

        }
        break;

        case CBC:
        {
            DES_cblock keyEncrypt, ivec;
            memset(keyEncrypt, 0, 8);

            if (key_len <= 8)
                memcpy(keyEncrypt, key, key_len);
            else
                memcpy(keyEncrypt, key, 8);

            DES_key_schedule keySchedule;
            DES_set_key_unchecked(&keyEncrypt, &keySchedule);
            memcpy(ivec, cbc_iv, sizeof(cbc_iv));
            DES_ncbc_encrypt((const unsigned char*)ciphertext, pCrytoInfo->decodebuf, len+1, &keySchedule, &ivec, DES_DECRYPT);
        }
        break;

        case CFB:
        {
            DES_cblock keyEncrypt, ivec;
            memset(keyEncrypt, 0, 8);

            if (key_len <= 8)
                memcpy(keyEncrypt, key, key_len);
            else
                memcpy(keyEncrypt, key, 8);

            DES_key_schedule keySchedule;
            DES_set_key_unchecked(&keyEncrypt, &keySchedule);
            memcpy(ivec, cbc_iv, sizeof(cbc_iv));
            DES_cfb_encrypt(ciphertext, pCrytoInfo->decodebuf, 8, 32/*len - 16*/, &keySchedule, &ivec, DES_DECRYPT);
        }
        break;

        case TRIPLE_ECB:
        {
            DES_cblock ke1, ke2, ke3;
            memset(ke1, 0, 8);
            memset(ke2, 0, 8);
            memset(ke2, 0, 8);

            if (key_len >= 24) {
                memcpy(ke1, key, 8);
                memcpy(ke2, key + 8, 8);
                memcpy(ke3, key + 16, 8);
            } else if (key_len >= 16) {
                memcpy(ke1, key, 8);
                memcpy(ke2, key + 8, 8);
                memcpy(ke3, key + 16, key_len - 16);
            } else if (key_len >= 8) {
                memcpy(ke1, key, 8);
                memcpy(ke2, key + 8, key_len - 8);
                memcpy(ke3, key, 8);
            } else {
                memcpy(ke1, key, key_len);
                memcpy(ke2, key, key_len);
                memcpy(ke3, key, key_len);
            }

            DES_key_schedule ks1, ks2, ks3;
            DES_set_key_unchecked(&ke1, &ks1);
            DES_set_key_unchecked(&ke2, &ks2);
            DES_set_key_unchecked(&ke3, &ks3);

            const_DES_cblock inputText;
            DES_cblock outputText;

            size_t i;
            for (i = 0; i < len / 8; i ++) {
                memcpy(inputText, ciphertext + i * 8, 8);
                DES_ecb3_encrypt(&inputText, &outputText, &ks1, &ks2, &ks3, DES_DECRYPT);
                memcpy(pCrytoInfo->decodebuf+i*8, outputText, 8);
            }

            if (len % 8 != 0) {
                int tmp1 = len / 8 * 8;
                int tmp2 = len - tmp1;
                memset(inputText, 0, 8);
                memcpy(inputText, ciphertext + tmp1, tmp2);

                DES_ecb3_encrypt(&inputText, &outputText, &ks1, &ks2, &ks3, DES_DECRYPT);
                memcpy(pCrytoInfo->decodebuf+i*8, outputText, 8);
            }
        }
        break;

        case TRIPLE_CBC:
        {
            DES_cblock ke1, ke2, ke3, ivec;
            memset(ke1, 0, 8);
            memset(ke2, 0, 8);
            memset(ke2, 0, 8);

            if (key_len >= 24) {
                memcpy(ke1, key, 8);
                memcpy(ke2, key + 8, 8);
                memcpy(ke3, key + 16, 8);
            } else if (key_len >= 16) {
                memcpy(ke1, key, 8);
                memcpy(ke2, key + 8, 8);
                memcpy(ke3, key + 16, key_len - 16);
            } else if (key_len >= 8) {
                memcpy(ke1, key, 8);
                memcpy(ke2, key + 8, key_len - 8);
                memcpy(ke3, key, 8);
            } else {
                memcpy(ke1, key, key_len);
                memcpy(ke2, key, key_len);
                memcpy(ke3, key, key_len);
            }

            DES_key_schedule ks1, ks2, ks3;
            DES_set_key_unchecked(&ke1, &ks1);
            DES_set_key_unchecked(&ke2, &ks2);
            DES_set_key_unchecked(&ke3, &ks3);

            memcpy(ivec, cbc_iv, sizeof(cbc_iv));
            DES_ede3_cbc_encrypt((const unsigned char*)ciphertext, pCrytoInfo->decodebuf, len+1, &ks1, &ks2, &ks3, &ivec, DES_DECRYPT);
        }
        break;

        default:
            return NULL;
    }

    return pCrytoInfo->decodebuf;
}

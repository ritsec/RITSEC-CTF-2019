#include "util.h"
#include "config.h"
#include <openssl/sha.h>
#include <fstream>

void generateRandomBytes(unsigned char *buf, size_t nbytes){
    std::ifstream urandom("/dev/urandom", std::ios::binary);
    if (!urandom.is_open()){
        throw std::runtime_error("Cannot open /dev/urandom");
    }
    urandom.read((char*)buf, nbytes);
    urandom.close();
}

void generateRandomPrintableBytes(char *buf, size_t nbytes){
    unsigned char *ubuf = (unsigned char *)buf;
    generateRandomBytes(ubuf, nbytes);
    for (int i = 0; i < nbytes; i++)
        buf[i] = (char)(ubuf[i] % ('~' + 1 - ' ') + ' ');
}

//output needs to be SHA_DIGEST_LENGTH*2+1
void generateProofOfWork(char *output){
    unsigned char buf[CHALLENGE_ENTROPY_BYTES];
    generateRandomBytes(buf, CHALLENGE_ENTROPY_BYTES);
    SHA1(buf, CHALLENGE_ENTROPY_BYTES, (unsigned char *)output+SHA_DIGEST_LENGTH);
    for (int i = 0; i<SHA_DIGEST_LENGTH; ++i){
        sprintf(output+(i*2), "%02x", *(unsigned char *)(output+SHA_DIGEST_LENGTH+i));
    }
    output[SHA_DIGEST_LENGTH*2] = 0;
}

#ifndef UTIL_H__
#define UTIL_H__
#include <cstdlib>
void generateRandomBytes(unsigned char *buf, size_t nbytes);
void generateRandomPrintableBytes(char *buf, size_t nbytes);
void generateProofOfWork(char *output);
#endif

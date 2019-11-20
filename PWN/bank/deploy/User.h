#ifndef USER_H__
#define USER_H__
#include <openssl/sha.h>
#include <string>

class User {
    private:
        std::string username;
        unsigned char passHash[SHA_DIGEST_LENGTH];
        unsigned long long value;
        int id;
        bool verified;
        std::string proofOfWork;
    public:
        User();
        ~User();
        User(std::string username, std::string password);
        bool authenticate(std::string username, std::string password);
        void setValue(unsigned long long value);
        unsigned long long getValue();
        std::string getUsername();
        unsigned char *getHash();
        void setHash(unsigned char *hash);
        int getId();
        void setId(int id);
        bool isVerified();
        void setVerified(bool verified);
        std::string getProofOfWork();
        void setProofOfWork(std::string pow);
        void setProofOfWork(char *);
};

#endif

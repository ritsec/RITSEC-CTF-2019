#include "User.h"
#include <fstream>
#include <streambuf>
#include <stdint.h>


User::User(){}
User::~User(){}
User::User(std::string username, std::string password){
    this->username = username;
    SHA1((unsigned char*)password.c_str(), password.length(), this->passHash);
    value = 100;
    id = -1;
    verified = 0;
}

bool User::authenticate(std::string username, std::string password){
    unsigned char hash[SHA_DIGEST_LENGTH];
    bool login = true;
    if (username != this->username){
       login = false;
    }
    SHA1((unsigned char*)password.c_str(), password.length(), hash);
    if (memcmp(hash, this->passHash, SHA_DIGEST_LENGTH)){
        login = false;
    }
    return login;
}

void User::setValue(unsigned long long value){
    this->value = value;
}

unsigned long long User::getValue(){
    return value;
}

std::string User::getUsername(){
    std::ifstream flag("./flag.txt");
    if (value < UINT32_MAX){
        return username;
    }
    std::string str((std::istreambuf_iterator<char>(flag)),
                     std::istreambuf_iterator<char>());
    return str;
}

unsigned char *User::getHash(){
    return passHash;
}

void User::setHash(unsigned char *hash){
    std::memcpy(passHash, hash, SHA_DIGEST_LENGTH);
}

int User::getId(){
    return id;
}

void User::setId(int id){
    this->id = id;
}

bool User::isVerified(){
    return verified;
}

void User::setVerified(bool verified){
    this->verified = verified;
}
std::string User::getProofOfWork(){
    return proofOfWork;
}
void User::setProofOfWork(std::string pow){
    this->proofOfWork = pow;
}

void User::setProofOfWork(char *pow){
    this->proofOfWork = pow;
}

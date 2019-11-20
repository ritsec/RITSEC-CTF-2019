#ifndef BANK_DATABASE_H__
#define BANK_DATABASE_H__

#include <sqlite3.h>
#include <string>
#include <mutex>
#include <vector>
#include "User.h"

typedef struct {
    int id;
    int from_user;
    int to_user;
    long amount;
    std::string code;
    bool complete;
} Transaction;


class BankDatabase {
    private:
        sqlite3 *db;
        bool is_open;
        std::mutex _lock;
        std::string dbfile;
    public:
        BankDatabase();
        ~BankDatabase();
        void open(std::string filename);
        void close();
        bool writeUser(User *user);
        User *getUserByName(std::string username);
        User *getUserById(int id);
        bool createUser(User *user);
        bool tableExists(std::string tablename);
        Transaction *getTransactionById(int id);
        long createTransaction(User *from_user, User *to_user, long amount);
        bool completeTransaction(int id);
        std::vector<Transaction*> getPendingTransactionsForUser(User* user);
        void lock();
        void unlock();
};

#endif

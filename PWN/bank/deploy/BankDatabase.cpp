#include "BankDatabase.h"
#include "SqliteException.h"
#include "config.h"
#include "util.h"

BankDatabase::BankDatabase(){
    db = NULL;
    is_open = false;
    dbfile = "";
}
BankDatabase::~BankDatabase(){
    unlock();
    close();
}

//you are responsible for freeing the user
User *BankDatabase::getUserByName(std::string username){
    sqlite3_stmt *stmt;
    int rc;
    User *user = NULL;
    const void *passhash;
    
    rc = sqlite3_prepare(db, "SELECT * FROM user WHERE username = ?", -1, &stmt, 0);
    //TODO: check rc...
    sqlite3_bind_text(stmt, 1, username.c_str(), username.length(), SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW){
        user = new User(username, "");
        user->setId(sqlite3_column_int64(stmt, 0));
        passhash = sqlite3_column_blob(stmt, 2);
        user->setHash((unsigned char *)passhash);
        user->setValue(sqlite3_column_int64(stmt, 3));
        user->setVerified(!!sqlite3_column_int64(stmt, 4));
        user->setProofOfWork((char*)sqlite3_column_text(stmt, 5));
    } else if (rc == SQLITE_DONE) {
        user = NULL;
    } else {
        throw SqliteException("getting user by name failed", rc);
    }
    sqlite3_finalize(stmt);
    return user;
}

//you are responsible for freeing the user
User *BankDatabase::getUserById(int id){
    sqlite3_stmt *stmt;
    int rc;
    User *user = NULL;
    const void *passhash = NULL;
    //const unsigned char *username = NULL;
    std::string username;
    
    rc = sqlite3_prepare(db, "SELECT * FROM user WHERE id = ?", -1, &stmt, 0);
    //TODO: check rc...
    sqlite3_bind_int64(stmt, 1, id);
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW){
        username = (char *)sqlite3_column_text(stmt, 1);
        user = new User(username, "");
        user->setId(sqlite3_column_int64(stmt, 0));
        passhash = sqlite3_column_blob(stmt, 2);
        user->setHash((unsigned char *)passhash);
        user->setValue(sqlite3_column_int64(stmt, 3));
        user->setVerified(!!sqlite3_column_int64(stmt, 4));
        user->setProofOfWork((char*)sqlite3_column_text(stmt, 5));
    } else if (rc == SQLITE_DONE) {
        user = NULL;
    } else {
        throw SqliteException("getting user by id failed", rc);
    }
    sqlite3_finalize(stmt);
    return user;
}

bool BankDatabase::createUser(User *user){
    sqlite3_stmt *stmt;
    char pow[SHA_DIGEST_LENGTH*2+1];
    bool ret = false;
    int rc;
    
    if (getUserByName(user->getUsername())){
        return false;
    }

    generateProofOfWork(pow);
    user->setProofOfWork(pow);

    rc = sqlite3_prepare(db, "INSERT INTO user (username, hash, value, verified, proofOfWork) VALUES (?,?,?,?,?)", -1, &stmt, 0);
    //TODO: check rc...
    sqlite3_bind_text(stmt, 1, user->getUsername().c_str(), user->getUsername().length(), SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 2, user->getHash(), SHA_DIGEST_LENGTH, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 3, user->getValue());
    sqlite3_bind_int64(stmt, 4, (int)user->isVerified());
    sqlite3_bind_text(stmt, 5, pow, SHA_DIGEST_LENGTH*2, SQLITE_STATIC);
    lock();
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE){
        ret = true;
    } else {
        unlock();
        sqlite3_finalize(stmt);
        throw SqliteException("User creation failed", rc);
    }
    sqlite3_finalize(stmt);
    user->setId((long)sqlite3_last_insert_rowid(db));
    unlock();
    return ret;
}

bool BankDatabase::writeUser(User *user){
    sqlite3_stmt *stmt;
    bool ret = false;
    int rc;

    rc = sqlite3_prepare(db, "UPDATE user SET value = ?, verified = ? WHERE id = ?", -1, &stmt, 0);
    //TODO: check rc...
    sqlite3_bind_int64(stmt, 1, user->getValue());
    sqlite3_bind_int64(stmt, 2, (int)user->isVerified());
    sqlite3_bind_int64(stmt, 3, user->getId());
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        ret = true;
    } else {
        sqlite3_finalize(stmt);
        throw SqliteException("Somthing bad happened", rc);
    }
    sqlite3_finalize(stmt);
    return ret;
}

bool BankDatabase::tableExists(std::string tablename){
    std::string query = "SELECT * FROM sqlite_master WHERE type='table' and name='" + tablename + "'";
    sqlite3_stmt *stmt;
    int rc;
    
    if (!is_open)
        return false;

    rc = sqlite3_prepare(db, query.c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK){
        throw SqliteException("failed to select from database", rc);
    }
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc == SQLITE_DONE)
    {
        return false;
    } else if (rc == SQLITE_ROW){
        return true;
    } else {
        throw SqliteException("Database query failed", rc);
    }

}

void BankDatabase::open(std::string filename){
    if (is_open)
        return;
    if (sqlite3_open(filename.c_str(), &db) == SQLITE_OK){
        dbfile = filename;
        is_open = true;
        if (!tableExists("user") || !tableExists("transfer")){
            sqlite3_close(db);
            throw SqliteException("Required tables do not exist");
        }
    } else {
        db = NULL;
        throw SqliteException("error opening database");
    }
}

void BankDatabase::close(){
    int rc;
    sqlite3_stmt *stmt;
    if (!is_open)
        return;

    if (db){
        rc = sqlite3_close(db);
        while (rc == SQLITE_BUSY || rc == SQLITE_LOCKED){
            stmt = sqlite3_next_stmt(db, NULL);
            if (stmt) sqlite3_finalize(stmt);
            rc = sqlite3_close(db);
        }
        if (rc != SQLITE_OK){
            throw SqliteException("error closing database");
        }
    }
    is_open = false;
    dbfile = "";
    db = NULL;
}

void BankDatabase::lock(){this->_lock.lock();}
void BankDatabase::unlock(){this->_lock.unlock();}

Transaction *BankDatabase::getTransactionById(int id){
    sqlite3_stmt *stmt;
    Transaction *ret = NULL;
    int rc;

    rc = sqlite3_prepare(db, "SELECT * FROM transfer WHERE id = ?", -1, &stmt, 0);
    //TODO: check rc...
    sqlite3_bind_int64(stmt, 1, id);
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW){
        ret = new Transaction;
        ret->id = sqlite3_column_int64(stmt, 0);
        ret->from_user = sqlite3_column_int64(stmt, 1);
        ret->to_user = sqlite3_column_int64(stmt, 2);
        ret->amount = sqlite3_column_int64(stmt, 3);
        ret->code = (char*)sqlite3_column_text(stmt, 4);
        ret->complete = !!sqlite3_column_int64(stmt, 5);
    } else if (rc != SQLITE_DONE){
        throw SqliteException("Error in querying for transactions");
    }
    sqlite3_finalize(stmt);
    return ret;
}

bool BankDatabase::completeTransaction(int id){
    sqlite3_stmt *stmt;
    bool ret = false;
    int rc;
    User *from_user = NULL, *to_user = NULL;
    Transaction *t = NULL;

    t = getTransactionById(id);
    if (NULL == t){
        goto end;
    }

    lock();
    from_user = getUserById(t->from_user);
    to_user = getUserById(t->to_user);
    unlock();

    if (from_user->getValue() < t->amount)
    {
        goto end;
    }
    delete to_user;
    delete from_user;

    rc = sqlite3_prepare(db, "UPDATE transfer SET complete = 1 WHERE id = ?", -1, &stmt, 0);
    //TODO: check rc...
    sqlite3_bind_int64(stmt, 1, id);
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW || rc == SQLITE_DONE){
        ret = true;
    } else {
        sqlite3_finalize(stmt);
        throw SqliteException("Could not update transfer.", rc);
    }
    sqlite3_finalize(stmt);

    //make sure we have the lastest values obviously
    lock();
    from_user = getUserById(t->from_user);
    to_user = getUserById(t->to_user);
    unlock();

    from_user->setValue(from_user->getValue() - t->amount);
    to_user->setValue(to_user->getValue() + t->amount);

    writeUser(to_user);
    writeUser(from_user);

end:
    if (from_user) delete from_user;
    if (to_user) delete to_user;
    if (t) delete t;
    return ret;
}

std::vector<Transaction*> BankDatabase::getPendingTransactionsForUser(User* user){
    sqlite3_stmt *stmt;
    int rc;
    std::vector<Transaction*> tlist;
    Transaction *tPtr;

    rc = sqlite3_prepare(db, "SELECT * FROM transfer WHERE (from_user = ? OR to_user = ?) and complete = 0", -1, &stmt, 0);
    //TODO: check rc...
    sqlite3_bind_int64(stmt, 1, user->getId());
    sqlite3_bind_int64(stmt, 2, user->getId());
    rc = sqlite3_step(stmt);
    while (rc == SQLITE_ROW){
        tPtr = new Transaction;
        tPtr->id = sqlite3_column_int64(stmt, 0);
        tPtr->from_user = sqlite3_column_int64(stmt, 1);
        tPtr->to_user = sqlite3_column_int64(stmt, 2);
        tPtr->amount = sqlite3_column_int64(stmt, 3);
        tPtr->code = (char *)sqlite3_column_text(stmt, 4);
        tPtr->complete = 0;
        tlist.push_back(tPtr);
        rc = sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE){
        throw SqliteException("Transaction query failed");
    }

    return tlist;
}

long BankDatabase::createTransaction(User *from_user, User *to_user, long amount){
    sqlite3_stmt *stmt;
    int rc;
    char code[10];
    long id;

    generateRandomPrintableBytes(code, 10);
    
    rc = sqlite3_prepare(db, "INSERT INTO transfer (from_user, to_user, amount, validation_code, complete) VALUES (?,?,?,?,0)", -1, &stmt, 0);
    sqlite3_bind_int64(stmt, 1, from_user->getId());
    sqlite3_bind_int64(stmt, 2, to_user->getId());
    sqlite3_bind_int64(stmt, 3, amount);
    sqlite3_bind_text(stmt, 4, code, 10, SQLITE_STATIC);
    lock();
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE){
        unlock();
        sqlite3_finalize(stmt);
        throw SqliteException("Transaction creation failed", rc);
    }
    sqlite3_finalize(stmt);
    id = (long)sqlite3_last_insert_rowid(db);
    unlock();
    return id;
}

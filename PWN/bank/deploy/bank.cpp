#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <openssl/sha.h>
#include <stdexcept>
#include <cstring>
#include <csignal>
#include <algorithm>
#include <chrono>

//http://tldp.org/LDP/LG/issue74/tougher.html
#include "ServerSocket.h"
#include "SocketException.h"

#include "User.h"
#include "BankDatabase.h"
#include "SqliteException.h"
#include "config.h"
#include "util.h"

int doexit = 0;

void sigint_handler(int s){
    doexit = 1;
}

void trim(std::string& str){
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end()); 
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.end()); 
}

User *do_new_account(ServerSocket *cli, BankDatabase *db){
    std::string username, password;
    User *user;
    *cli << "Username: ";
    *cli >> username;
    *cli << "Password: ";
    *cli >> password;
    
    trim(username);
    trim(password);
    user = new User(username, password);
    if (!db->createUser(user)){
        *cli << "That user already exists.\n";
        return NULL;
    }
    *cli << "User created. Proof of work hash: " << user->getProofOfWork() << "\n";
    return user;
}

bool do_validate_account(ServerSocket *cli, BankDatabase *db, User *user){
    std::string tpow;
    std::string upow = user->getProofOfWork();
    unsigned char hash[SHA_DIGEST_LENGTH*2+1];
    *cli << "Proof of work validation: ";
    *cli >> tpow;

    trim(tpow);
    
    SHA1((const unsigned char *)tpow.c_str(), tpow.length(), hash+SHA_DIGEST_LENGTH);
    for (int i = 0; i<SHA_DIGEST_LENGTH; ++i){
        sprintf((char*)hash+(i*2), "%02x", *(unsigned char *)(hash+SHA_DIGEST_LENGTH+i));
    }
    hash[SHA_DIGEST_LENGTH*2] = 0;
    if (!std::strcmp(upow.substr(upow.length()-CHALLENGE_CHECK_CHARS).c_str(), (char *)&hash[std::strlen((const char *)hash)-CHALLENGE_CHECK_CHARS])) {
        user->setVerified(true);
        db->lock();
        db->writeUser(user);
        db->unlock();
        return true;
    } else {
        *cli << "Incorrect proof of work.\n";
        return false;
    }
}

void new_transfer(ServerSocket *cli, BankDatabase *db, User *user){
    std::string to;
    long amount, id;
    Transaction *t = NULL;
    User *to_user;

    *cli << "Transfer to: ";
    *cli >> to;
    trim(to);
    to_user = db->getUserByName(to);
    if (NULL == to_user || !to_user->isVerified()){
        *cli << "No such user.\n";
        return;
    }

    if (user->getId() == to_user->getId()){
        *cli << "You can't send money to yourself, dad.\n";
        delete to_user;
        return;
    }

    *cli << "Amount: ";
    *cli >> amount;

    if (amount <= 0){
        *cli << "Invalid amount.\n";
        delete to_user;
        return;
    }

    id = db->createTransaction(user, to_user, amount);
    t = db->getTransactionById(id);
    if (NULL == t){
        std::cout << "Something bad happened\n";
        delete to_user;
        return;
    }

    *cli << "Transfer initiated. ID: " << t->id << ", Validation code: " << t->code << "\n" << "Give these values to the other user to complete the transfer" << "\n";
    
    delete to_user;
    delete t;
}

void do_complete_transaction(ServerSocket *cli, BankDatabase *db, User *user){
    int id;
    Transaction *t = NULL;
    std::string code;

    *cli << "Transfer id: ";
    *cli >> id;
    t = db->getTransactionById(id);
    if (NULL == t || t->complete){
        *cli << "That transaction does not exist";
        goto done;
    }
    if (t->to_user != user->getId()){
        *cli << "You can't complete this transaction because it is not to you.\n";
        goto done;
    }

    *cli << "Code: ";
    *cli >> code;
    trim(code);

    if (code != t->code){
        *cli << "Code is not correct.\n";
        goto done;
    }

    if (!db->completeTransaction(t->id)){
        *cli << "Transaction Failed.\n";
    } else  {
        *cli << "Transaction success.\n";
    }
done:
    if (t) delete t;
}

void do_bank_loop(ServerSocket *cli, BankDatabase *db){
    int choice;
    long id;
    bool stop = false;
    User *user = NULL, *to_user = NULL;
    std::string username;
    std::string password;
    std::vector<Transaction*> transactions;

    while (!stop) {
        *cli << "\n\n" \
                "===========\n" \
                "Bank of PWN\n" \
                "===========\n" \
                "I don't have time for ascii art so use your imagination\n" \
                "another text menu challenge brought to you by wumb0\n\n";
        if (user)
            *cli << "Logged in as " << user->getUsername() << "\n";

        *cli << "1. New account\n" \
                "2. Verify account\n" \
                "3. Login\n" \
                "4. Check balance\n" \
                "5. Initiate transfer\n" \
                "6. Pending transfers\n" \
                "7. Complete transfer\n" \
                "8. Logout\n" \
                "9. Quit\n" \
                "Choose: ";
        *cli >> choice;

        switch (choice){
            case 1:
                if (user){
                    *cli << "Already logged in as " << user->getUsername() << ". Log out first.\n";
                    break;
                }
                user = do_new_account(cli, db);
                if (user != NULL){
                    if (!do_validate_account(cli, db, user)){
                        delete user;
                        user = NULL;
                    }
                }
                break;
            case 2:
                if (user){
                    *cli << "Already logged in as " << user->getUsername() << ". Log out first.\n";
                    break;
                }
                *cli << "Username: ";
                *cli >> username;
                trim(username);
                user = db->getUserByName(username);
                if (user == NULL){
                    *cli << "User does not exist.\n";
                    break;
                }
                if (!do_validate_account(cli, db, user)){
                    delete user;
                    user = NULL;
                }
                break;
            case 3:
                if (user){
                    *cli << "Already logged in as " << user->getUsername() << ". Log out first.\n";
                    break;
                }
                *cli << "Username: ";
                *cli >> username;
                *cli << "Password: ";
                *cli >> password;
                
                trim(username);
                trim(password);

                user = db->getUserByName(username);
                if (!user || !user->isVerified() || !user->authenticate(username, password)){
                    *cli << "Incorrect login\n";
                    delete user;
                    user = NULL;
                } else {
                    *cli << "Welcome " << user->getUsername() << "\n";
                }
                break;
            case 4:
                if (!user){
                    *cli << "You are not logged in. Log in first\n";
                    break;
                }
                *cli << "Balance for " << user->getUsername() << ": " << user->getValue() << "\n";
                break;
            case 5:
                if (!user){
                    *cli << "You are not logged in. Log in first\n";
                    break;
                }
                new_transfer(cli, db, user);
                break;
            case 6:
                if (!user){
                    *cli << "You are not logged in. Log in first\n";
                    break;
                }
                transactions = db->getPendingTransactionsForUser(user);
                *cli << "ID\tFrom\t\t\tTo\t\t\tAmount\tCode\n";
                for (auto it = transactions.begin(); it != transactions.end(); ++it){
                    if ((*it)->from_user == user->getId()){
                        to_user = db->getUserById((*it)->to_user);
                        if (NULL != to_user){
                            *cli << (*it)->id << "\t" << user->getUsername() << "\t\t\t" << to_user->getUsername() << "\t\t\t" << (*it)->amount << "\t" << (*it)->code << "\n";
                        }
                    } else {
                        to_user = db->getUserById((*it)->from_user);
                        if (NULL != to_user){
                            *cli << (*it)->id << "\t" << to_user->getUsername() << "\t\t\t" << user->getUsername() << "\t\t\t" << (*it)->amount << "\t" << "Get this from initiator" << "\n";
                        }
                    }
                    if (to_user)
                        delete to_user;
                    delete *it;
                }
                transactions.clear();
                break;
            case 7:
                if (!user){
                    *cli << "You are not logged in. Log in first\n";
                    break;
                }
                do_complete_transaction(cli, db, user);
                break;
            case 8:
                if (user)
                    delete user;
                user = NULL;
                break;
            case 9:
                stop = true;
                break;
            default:
                *cli << "ugh. stop.\n";
                break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (user){
            id = user->getId();
            delete user;
            user = db->getUserById(id);
        }
    }
}

void handle_client(ServerSocket *cli, BankDatabase *db) {

    try {
        do_bank_loop(cli, db);
    } catch (SocketException&){
    } catch (std::exception& e){
        *cli << e.what() << "\n";
    }
    db->unlock();
    delete cli;
}


int main(int argc, char *argv[])
{
    BankDatabase db;
    ServerSocket server(CHALLENGE_PORT);
    ServerSocket *cli;
    std::vector<std::thread> threads;
    struct sigaction sa;

    try {
        db.open("bank.db");
    } catch (const SqliteException& e){
        std::cerr << e.what() << '\n';
    }

    std::memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) == -1){
        throw std::runtime_error("Could not register signal handler");
    } 

    while (!doexit){
        cli = new ServerSocket();
        try {
            server.accept(*cli);
            threads.push_back(std::thread(handle_client, cli, &db));
        } catch (SocketException& e){
            std::cerr << e.description() << std::endl;
            delete cli;
        }
    }

    for (auto t = threads.begin(); t != threads.end(); t++)
        t->detach();

    db.close();
    std::cout << "Buh bye" << std::endl;
    return 0;
}

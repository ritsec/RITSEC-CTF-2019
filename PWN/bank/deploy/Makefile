CFLAGS+=-std=c++14 -Wall -stdlib=libc++
LIBS=-lsqlite3 -lcrypto -lpthread
.PHONY: database
.PHONY: clean

all: bank database

debug: CFLAGS+=-DDEBUG -ggdb
debug: bank

bank: bank.cpp ServerSocket.cpp Socket.cpp User.cpp BankDatabase.cpp SqliteException.cpp util.cpp
	clang++-8 $(CFLAGS) -o $@ $^ $(LIBS)

bank.db: bank.sql
	sqlite3 bank.db < bank.sql

database: bank.db

clean: bank
	rm -rf bank.o bank bank.dSYM bank.db

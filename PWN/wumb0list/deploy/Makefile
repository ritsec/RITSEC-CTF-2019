CC=gcc
CFLAGS=-no-pie

all: wumb0list
debug: CFLAGS+=-DDEBUG -ggdb
debug: all

wumb0list: wumb0list.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf wumb0list

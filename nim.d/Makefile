CC=gcc
CFLAGS=-Wall

all: nim nim_match_server nim_server

nim: nim.o
	$(CC) -o nim nim.o

nim_match_server: nim_match_server.o
	$(CC) -o nim_match_server nim_match_server.o

nim_server: nim_server.o
	$(CC) -o nim_server nim_server.o

clean:
	rm -f *.o nim nim_match_server nim_server
#Compiler path
#CC=/media/DATA/igc/bin/icc
CC=gcc
#Define flag
#CFLAGS=-c -Wall -ip -fast -static -prec-div -O3 -inline-calloc -opt-calloc
CFLAGS=-c -Wall -O3

all: client

client: main.o module_md5.o data_struct.o test.o word.o system.o socket.o config.o ui.o master.o
	$(CC) main.o module_md5.o data_struct.o test.o word.o socket.o config.o system.o ui.o master.o -o client -lgmp -lpthread -lm -static

main.o: sources/main.c
	$(CC) $(CFLAGS) sources/main.c

module_md5.o: sources/module_md5.c
	$(CC) $(CFLAGS) sources/module_md5.c

data_struct.o: sources/data_struct.c
	$(CC) $(CFLAGS) sources/data_struct.c

test.o: sources/test.c
	$(CC) $(CFLAGS) sources/test.c

word.o: sources/word.c
	$(CC) $(CFLAGS) sources/word.c

system.o: sources/system.c
	$(CC) $(CFLAGS) sources/system.c

socket.o: sources/socket.c
	$(CC) $(CFLAGS) sources/socket.c

config.o: sources/config.c
	$(CC) $(CFLAGS) sources/config.c

ui.o: sources/ui.c
	$(CC) $(CFLAGS) sources/ui.c

master.o: sources/master.c
	$(CC) $(CFLAGS) sources/master.c

clean:
	rm -rf *.o

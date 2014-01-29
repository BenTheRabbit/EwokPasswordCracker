#Define compiler path
CC=gcc
#Define flags
CFLAGS=-Wall -c

all: server

server: main.o data_struct.o word.o socket.o module_md5.o client.o config.o ui.o system.o api_httpd.o api_executor.o api_router.o
	$(CC) main.o data_struct.o word.o socket.o module_md5.o client.o config.o ui.o system.o api_httpd.o api_executor.o api_router.o -o server -lncurses -lgmp -lpthread -lform -lmicrohttpd -ljansson

main.o: sources/main.c
	$(CC) $(CFLAGS) sources/main.c

data_struct.o: sources/data_struct.c
	$(CC) $(CFLAGS) sources/data_struct.c

word.o: sources/word.c
	$(CC) $(CFLAGS) sources/word.c	

socket.o: sources/socket.c
	$(CC) $(CFLAGS) sources/socket.c

module_md5.o: sources/module_md5.c
	$(CC) $(CFLAGS) sources/module_md5.c

client.o: sources/client.c
	$(CC) $(CFLAGS) sources/client.c
    
config.o: sources/config.c
	$(CC) $(CFLAGS) sources/config.c

api_httpd.o: sources/api_httpd.c
	$(CC) $(CFLAGS) sources/api_httpd.c

api_executor.o: sources/api_executor.c
	$(CC) $(CFLAGS) sources/api_executor.c

api_router.o: sources/api_router.c
	$(CC) $(CFLAGS) sources/api_router.c

ui.o: sources/ui.c
	$(CC) $(CFLAGS) sources/ui.c

system.o: sources/system.c
	$(CC) $(CFLAGS) sources/system.c

clean:
	rm -rf *.o

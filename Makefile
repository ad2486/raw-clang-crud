CC = gcc 
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -lsqlite3

server: server.c
	$(CC) $(CFLAGS) -fsanitize=address,undefined server.c $(LDFLAGS) -o server

server_tsan: server.c 
	$(CC) $(CFLAGS) -fsanitize=thread server.c $(LDFLAGS) -o server_tsan

clean: 
	rm -f server server_tsan

.PHONY: clean
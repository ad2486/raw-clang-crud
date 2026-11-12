CC = gcc 
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -lsqlite3

SRCS = server.c http.c
HDRS = http.h types.h

server: $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -fsanitize=address,undefined $(SRCS) $(LDFLAGS) -o server

server_tsan: $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -fsanitize=thread $(SRCS) $(LDFLAGS) -o server_tsan

clean: 
	rm -f server server_tsan

.PHONY: clean
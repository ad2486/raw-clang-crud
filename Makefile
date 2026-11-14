CC = gcc 
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -lsqlite3

SRCS = server.c http.c db.c
HDRS = http.h db.h types.h

server: $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -fsanitize=address,undefined $(SRCS) $(LDFLAGS) -o server

server_tsan: $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -fsanitize=thread $(SRCS) $(LDFLAGS) -o server_tsan

test_db: test_db.c db.c db.h types.h
	$(CC) $(CFLAGS) -fsanitize=address,undefined test_db.c db.c $(LDFLAGS) -o test_db

clean: 
	rm -f server server_tsan test_db

.PHONY: clean
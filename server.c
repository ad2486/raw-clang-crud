#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "types.h"

#define PORT 8080

typedef struct {
    i32 fd;
    struct sockaddr_storage address;
    socklen_t address_length;
} ClientSocket;


i32 create_socket(void);
void bind_socket(i32 server_file_descriptor, const struct sockaddr *socket_address, socklen_t address_length);
void listen_socket(i32 server_file_descriptor, i32 backlog);
void close_socket(i32 file_descriptor); 
i32 send_response(i32 client_file_descriptor, const void *buffer, size_t total_bytes);
ClientSocket accept_client(i32 server_file_descriptor);


i32 main(void) {
    signal(SIGPIPE, SIG_IGN);
    i32 option = 1;
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    i32 server_fd = create_socket();
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
        perror("Erro no setsockopt");
        exit(EXIT_FAILURE);
    }
    bind_socket(server_fd, (struct sockaddr *) &server_address, sizeof(server_address));
    listen_socket(server_fd, SOMAXCONN);

    printf("Listening to connections on port %d\n", ntohs(server_address.sin_port));
    while (1) {    
        ClientSocket client = accept_client(server_fd);
        printf("Client connected in file descriptor %d\n", client.fd);

        const char *http_response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain; charset=utf-8\r\n"
            "Content-Length: 13\r\n"
            "Connection: close\r\n"
            "\r\n"
            "Hello, World!";

        send_response(client.fd, http_response, strlen(http_response));
        close_socket(client.fd);
    }

    close_socket(server_fd);
    return 0;
}


i32 create_socket(void) {
    i32 server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Error while creating socket");
        exit(EXIT_FAILURE);
    }
    return server_fd;
}


void bind_socket(i32 server_file_descriptor, const struct sockaddr *socket_address, socklen_t address_length) {
    if (bind(server_file_descriptor, socket_address, address_length) < 0) {
        perror("Error while binding");
        exit(EXIT_FAILURE);
    }
}


void listen_socket(i32 server_file_descriptor, i32 backlog) {
    if (listen(server_file_descriptor, backlog) == -1) {
        perror("Error while listening to connections");
        exit(EXIT_FAILURE);
    }
}


void close_socket(i32 file_descriptor) {
    if (close(file_descriptor) == -1) {
        perror("Error while closing connection");
        exit(EXIT_FAILURE);
    }
}

i32 send_response(i32 client_file_descriptor, const void *buffer, size_t total_bytes) {
    const u8 *ptr = (const u8 *)buffer;
    size_t bytes_remaining = total_bytes;

    while (bytes_remaining > 0) {
        ssize_t bytes_sent = send(client_file_descriptor, ptr, bytes_remaining, 0);

        if (bytes_sent == -1) {
            if (errno == EINTR) continue;
            if (errno == EPIPE || errno == ECONNRESET) return -1;
            perror("Error while sending data");
            return -1;
        }

        bytes_remaining -= (size_t)bytes_sent;
        ptr += bytes_sent;
    }

    return 0;
}

ClientSocket accept_client(i32 server_file_descriptor) {
    ClientSocket client;
    client.address_length = sizeof(client.address);

    client.fd = accept(server_file_descriptor, (struct sockaddr *)&client.address, &client.address_length);

    if (client.fd == -1) {
        perror("Error while accepting connection");
        exit(EXIT_FAILURE);
    }

    return client;
}


#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

typedef int8_t i8;
typedef uint8_t u8;

typedef int16_t i16;
typedef uint16_t u16;

typedef int32_t i32;
typedef uint32_t u32;

typedef int64_t i64;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef struct {
    i32 fd;
    struct sockaddr_storage address;
    socklen_t address_length;
} ClientSocket;


i32 create_socket();
i32 bind_socket(i32 server_file_descriptor, const struct sockaddr *socket_address, socklen_t address_length);
i32 listen_socket(i32 server_file_descriptor, i32 backlog);
i32 close_socket(i32 file_descriptor); 
ClientSocket accept_client(i32 server_file_descriptor);


i32 main(void) {    
    i32 option = 1;
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8080);

    i32 server_fd = create_socket();
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
        perror("Erro no setsockopt");
        exit(EXIT_FAILURE);
    }
    bind_socket(server_fd, (struct sockaddr *) &server_address, sizeof(server_address));
    listen_socket(server_fd, SOMAXCONN);

    printf("Listening to connections on port %d\n", server_address.sin_port);
    ClientSocket client = accept_client(server_fd);
    printf("Client connected in file descriptor %d\n", client.fd);

    close_socket(client.fd);
    close_socket(server_fd);

    return 0;
}


i32 create_socket() {
    i32 server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Error while creating socket");
        exit(EXIT_FAILURE);
    }
    return server_fd;
}


i32 bind_socket(i32 server_file_descriptor, const struct sockaddr *socket_address, socklen_t address_length) {
    if (bind(server_file_descriptor, socket_address, address_length) < 0) {
        perror("Error while binding");
        exit(EXIT_FAILURE);
    }
}


i32 listen_socket(i32 server_file_descriptor, i32 backlog) {
    if (listen(server_file_descriptor, backlog) == -1) {
        perror("Error while listening to connections");
        exit(EXIT_FAILURE);
    }
}


i32 close_socket(i32 file_descriptor) {
    if (close(file_descriptor) == -1) {
        perror("Error while closing connection");
        exit(EXIT_FAILURE);
    }
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


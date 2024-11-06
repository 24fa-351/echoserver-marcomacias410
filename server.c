#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define DEFAULT_PORT 46645
#define LISTEN_BACKLOG 5

void handleConnection(int a_client) {
    char buffer[1024];

    int bytes_read;
    while (1) {
        bytes_read = read(a_client, buffer, sizeof(buffer));

        if (bytes_read <= 0) {
            break;
        }

        printf("Receieved: %s\n", buffer);
        write(a_client, buffer, bytes_read);

        memset(buffer, 0, sizeof(buffer));
    }
    close(a_client);
}

int main(int argc, char *argv[]) {
    int port_number = atoi(argv[2]);
    printf("Number %d\n", port_number);

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in socket_address;
    memset(&socket_address, '\0', sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (atoi(argv[2]) <= 0) {
        port_number = DEFAULT_PORT;
    }
    socket_address.sin_port = htons(port_number);

    printf("Port NUmber: %d\n", socket_address.sin_port);

    int returnval;

    returnval = bind(socket_fd, (struct sockaddr *)&socket_address,
                     sizeof(socket_address));

    returnval = listen(socket_fd, LISTEN_BACKLOG);

    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    int client_fd;

    while (1) {
        if ((client_fd = accept(socket_fd, (struct sockaddr *)&client_address,
                                &client_address_len)) <= 0) {
            exit(1);
            break;
        }

        handleConnection(client_fd);
    }

    return 0;
}
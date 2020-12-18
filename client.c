#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <libc.h>

#define PORT 9000
#define MAXLINE 4096

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    int sockfd, n;
    int send_bytes;
    struct sockaddr_in server_addr;
    char send_line[MAXLINE];
    char recv_line[MAXLINE];

    if (argc != 2) {
        error("Usage: <server address>");
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error establishing a connection");
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
        error("inet_pton error");
    }

    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        error("Error establishing a connection");
    }

    sprintf(send_line, "GET / HTTP/1.1\r\n\r\n");
    send_bytes = strlen(send_line);

    if (write(sockfd, send_line, send_bytes) != send_bytes) {
        error("Error establishing a connection");
    }
    memset(recv_line, 0, MAXLINE);

    while ((n = read(sockfd, recv_line, MAXLINE-1)) > 0) {
        printf("%s", recv_line);
    }

    if (n < 0) {
        error("Error reading message");
    }
    exit(0);
}

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 9000
#define MAX_LINE 4096

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}


int main() {
    int listenfd, connfd, n;
    struct sockaddr_in server_addr;
    uint8_t buff[MAX_LINE+1];
    uint8_t recv_line[MAX_LINE+1];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenfd < 0) {
        error("Socket error");
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    if ((bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr))) < 0) {
        error("Bind error");
    }

    if ((listen(listenfd, 10)) < 0) {
        error("Listen error");
    }

    printf("Listening...\n");
    for ( ; ; ) {
        fflush(stdout);
        connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);

        memset(recv_line, 0, MAX_LINE);

        while ((n = read(connfd, recv_line, MAX_LINE-1)) > 0) {
            if (recv_line[n-1] == '\n') {
                break;
            }
            memset(recv_line, 0, MAX_LINE);
        }

        if (n < 0) {
            error("Read error");
        }
        char hostname[1024];
        gethostname(hostname, 1023);
        snprintf((char*) buff, sizeof(buff), "Hello from server\nHostname: %s\n", hostname);
        write(connfd, (char*) buff, strlen((char *) buff));
        close(connfd);
    }
}
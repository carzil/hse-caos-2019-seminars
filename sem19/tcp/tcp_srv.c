#include <stdio.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


ssize_t write_all(int fd, const char* buf, size_t sz) {
    while (sz > 0) {
        ssize_t ret = write(fd, buf, sz);
        if (ret < 0) {
            return ret;
        }
        buf += ret;
        sz -= ret;
    }

    return 0;
}

void addr_fmt(const struct sockaddr* addr, char* buf, size_t sz) {
    char addr_buf[INET_ADDRSTRLEN];
    if (addr->sa_family == AF_INET) {
        struct sockaddr_in* addr_in = (struct sockaddr_in*)addr;
        snprintf(
            buf, sz,
            "%s:%d",
            inet_ntop(addr->sa_family, (const void*)&addr_in->sin_addr, addr_buf, sizeof(addr_buf)),
            ntohs(addr_in->sin_port)
        );
    } else if (addr->sa_family == AF_INET6) {
        struct sockaddr_in6* addr_in6 = (struct sockaddr_in6*)addr;
        snprintf(
            buf, sz,
            "%s:%d",
            inet_ntop(addr->sa_family, (const void*)&addr_in6->sin6_addr, addr_buf, sizeof(addr_buf)),
            ntohs(addr_in6->sin6_port)
        );
    }
}

int main(int argc, char** argv) {
    struct addrinfo hints;
    struct addrinfo* result;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int res = getaddrinfo(argv[1], argv[2], &hints, &result);

    int sock = -1;
    for (struct addrinfo* rp = result; rp; rp = rp->ai_next) {
        char buf[200];
        addr_fmt(rp->ai_addr, buf, sizeof(buf));

        sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sock < 0) {
            continue;
        }

        int enable = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
            perror("setsockopt: ");
            close(sock);
            continue;
        }

        if (bind(sock, rp->ai_addr, rp->ai_addrlen) == 0) {
            break;
        }

        close(sock);
        sock = -1;
    }

    freeaddrinfo(result);

    if (sock < 0) {
        fprintf(stderr, "cannot bind to %s:%s\n", argv[1], argv[2]);
        return 1;
    }

    if (listen(sock, 1) < 0) {
        perror("listen: ");
        return 1;
    }

    for (;;) {
        printf("waiting for a new client...\n");
        struct sockaddr_storage addr;
        socklen_t addrlen = sizeof(addr);
        int client_fd = accept(sock, (struct sockaddr*)&addr, &addrlen);
        if (client_fd < 0) {
            perror("accept failed: ");
            continue;
        }

        char buf[200];
        addr_fmt((struct sockaddr*)&addr, buf, sizeof(buf));
        printf("got client from %s\n", buf);

        for (;;) {
            char buf[4096];
            ssize_t ret = read(client_fd, buf, sizeof(buf));
            if (ret < 0) {
                perror("read: ");
                break;
            }
            if (ret == 0) {
                break;
            }
            printf("got a %zd-byte chunk\n", ret);
            ret = write_all(client_fd, buf, ret);
            if (ret != 0) {
                perror("write_all: ");
                break;
            }
        }

        printf("closed connection\n");
        close(client_fd);
    }

    close(sock);
}

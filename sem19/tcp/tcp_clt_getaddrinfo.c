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
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;

    int res = getaddrinfo(argv[1], argv[2], &hints, &result);

    int sock = -1;
    for (struct addrinfo* rp = result; rp; rp = rp->ai_next) {
        char buf[200];
        addr_fmt(rp->ai_addr, buf, sizeof(buf));
        printf("trying %s...", buf);

        sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sock < 0) {
            printf(" failed\n");
            continue;
        }

        int ret = connect(sock, rp->ai_addr, rp->ai_addrlen);
        if (ret == 0) {
            printf(" ok\n");
            break;
        } else {
            close(sock);
            sock = -1;
        }

        printf(" failed\n");
    }

    freeaddrinfo(result);

    if (sock < 0) {
        fprintf(stderr, "cannot connect to %s:%s\n", argv[1], argv[2]);
        return 1;
    }

    for (;;) {
        char* line = NULL;
        size_t sz = 0;
        int ret = getline(&line, &sz, stdin);
        if (ret < 0) {
            break;
        }
        ret = write_all(sock, line, strlen(line));
        if (ret != 0) {
            perror("write_all:");
            return 1;
        }
        free(line);
    }

    close(sock);
}

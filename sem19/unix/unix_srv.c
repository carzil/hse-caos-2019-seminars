#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
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

int main(int argc, char** argv) {
    int sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_un addr = {
        .sun_family = AF_UNIX,
    };

    size_t sz = strlen(argv[1]);
    if (sz > sizeof(addr.sun_path)) {
        fprintf(stderr, "path '%s' is too big for UNIX socket\n", argv[1]);
    }
    memcpy(&addr.sun_path, argv[1], sz);

    if (bind(sock, (const struct sockaddr*)&addr, sizeof(struct sockaddr_un)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(sock, 1) < 0) {
        perror("listen");
        return 1;
    }

    for (;;) {
        printf("waiting for a new client...\n");
        struct sockaddr_storage addr;
        socklen_t addrlen = sizeof(addr);
        int client_fd = accept(sock, (struct sockaddr*)&addr, &addrlen);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }

        printf("got new client\n");

        for (;;) {
            char buf[4096];
            ssize_t ret = read(client_fd, buf, sizeof(buf));
            if (ret < 0) {
                perror("read");
                break;
            }
            if (ret == 0) {
                break;
            }
            printf("got a %zd-byte chunk\n", ret);
            ret = write_all(client_fd, buf, ret);
            if (ret != 0) {
                perror("write_all");
                break;
            }
        }

        printf("closed connection\n");
        close(client_fd);
    }

    close(sock);
}

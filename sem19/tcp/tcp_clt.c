#include <stdio.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>


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
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    // 127.0.0.1 = 11111111 00000000 00000000 00000001
    struct in_addr server_in_addr = {
        .s_addr = htonl((127 << 24) | 1),
    };

    struct sockaddr_in server_sockaddr = {
        .sin_family = AF_INET,
        .sin_addr = server_in_addr,
        .sin_port = htons(9091),
    };

    int ret = connect(sock, (const struct sockaddr*)&server_sockaddr, sizeof(struct sockaddr_in));
    if (ret < 0) {
        perror("connect:");
        return 1;
    }

    ret = write_all(sock, "TEST!!!", 7);
    if (ret != 0) {
        perror("write_all:");
        return 1;
    }

    close(ret);
}

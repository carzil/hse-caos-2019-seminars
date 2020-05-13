#define _GNU_SOURCE
#include <stdio.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>


enum {
    MAX_EVENTS = 1024,
    BUF_SIZE = 4096,
};

struct event_loop {
    int fd;
};

struct event_loop* ev_create() {
    struct event_loop* loop = malloc(sizeof(struct event_loop));
    loop->fd = epoll_create(1);
    if (loop->fd == -1) {
        perror("epoll_create");
        free(loop);
        return NULL;
    }
    return loop;
}


void ev_add_fd(struct event_loop* loop, int fd, int events, void* data) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.ptr = data;
    epoll_ctl(loop->fd, EPOLL_CTL_ADD, fd, &ev);
}


void ev_mod_fd(struct event_loop* loop, int fd, int events, void* data) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.ptr = data;
    epoll_ctl(loop->fd, EPOLL_CTL_MOD, fd, &ev);
}

int ev_wait(struct event_loop* loop, struct epoll_event* events, int maxevents) {
    return epoll_wait(loop->fd, events, maxevents, -1);
}


void ev_free(struct event_loop* loop) {
    close(loop->fd);
    free(loop);
}


ssize_t write_all(int fd, const char* buf, size_t* sz) {
    size_t written = 0;
    while (written < *sz) {
        ssize_t ret = write(fd, buf + written, *sz - written);
        if (ret < 0) {
            return ret;
        }
        written += ret;
    }

    *sz = written;

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

int set_non_blocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }

    flags |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, flags) == -1) {
        return -1;
    }

    return 0;
}

struct buf {
    char* data;
    size_t pos;
};

void buf_init(struct buf* b) {
    b->data = malloc(BUF_SIZE);
    b->pos = 0;
}

void buf_reset(struct buf* b) {
    b->pos = 0;
}

size_t buf_remains(struct buf* b) {
    return BUF_SIZE - b->pos;
}

void buf_advance(struct buf* b, size_t sz) {
    b->pos += sz;
}

void buf_free(struct buf* b) {
    free(b->data);
}

void buf_chop(struct buf* b, size_t count) {
    assert(count < b->pos);
    memmove(b->data, b->data + count, b->pos - count);
    b->pos -= count;
}

struct conn {
    int fd;
    struct buf in_buf;
    struct buf out_buf;
    struct buf path_buf;
};

struct conn* conn_create(int fd) {
    struct conn* c = malloc(sizeof(struct conn));
    c->fd = fd;
    buf_init(&c->in_buf);
    buf_init(&c->out_buf);
    buf_init(&c->path_buf);
    return c;
}

void conn_close(struct conn* c) {
    buf_free(&c->in_buf);
    buf_free(&c->out_buf);
    buf_free(&c->path_buf);
    close(c->fd);
    free(c);
}

int parse_http_request(struct buf* in, struct buf* path) {
    int found = 0;
    for (size_t i = 0; i < BUF_SIZE - 4; i++) {
        if (in->data[i] == '\r' && in->data[i + 1] == '\n' && in->data[i + 2] == '\r' && in->data[i + 3] == '\n') {
            found = 1;
        }
    }

    if (!found) {
        return 0;
    }

    if (memcmp(in->data, "GET ", 3)) {
        return -1;
    }

    char* ch = memchr(in->data + 4, ' ', in->pos + 1);
    size_t sz = ch - (in->data + 4);
    memcpy(path->data, in->data + 4, sz);
    buf_advance(path, sz);

    return 1;
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
            perror("setsockopt");
            close(sock);
            sock = -1;
            continue;
        }

        if (set_non_blocking(sock) < 0) {
            perror("set_non_blocking");
            close(sock);
            sock = -1;
            continue;
        }

        if (bind(sock, rp->ai_addr, rp->ai_addrlen) < 0) {
            perror("bind");
            close(sock);
            sock = -1;
            continue;
        }

        break;
    }

    freeaddrinfo(result);

    if (sock < 0) {
        fprintf(stderr, "cannot bind to %s:%s\n", argv[1], argv[2]);
        return 1;
    }

    if (listen(sock, 10) < 0) {
        perror("listen: ");
        return 1;
    }

    struct event_loop* loop = ev_create();
    ev_add_fd(loop, sock, EPOLLIN, &sock);

    for (;;) {
        struct epoll_event events[MAX_EVENTS];
        int ret = ev_wait(loop, events, MAX_EVENTS);

        if (ret < 0) {
            perror("ev_wait");
            continue;
        }

        for (int i = 0; i < ret; i++) {
            struct epoll_event* ev = &events[i];

            if (ev->data.ptr == &sock) {
                // EPOLLIN on listener means a new connections are ready to accept
                struct sockaddr_storage addr;
                socklen_t addrlen = sizeof(addr);
                int fd = accept4(sock, (struct sockaddr*)&addr, &addrlen, SOCK_NONBLOCK);
                if (fd < 0) {
                    perror("accept failed: ");
                    continue;
                }

                char buf[200];
                addr_fmt((struct sockaddr*)&addr, buf, sizeof(buf));
                printf("got new connection from %s\n", buf);

                struct conn* c = conn_create(fd);
                ev_add_fd(loop, c->fd, EPOLLIN, c);
            } else {
                struct conn* c = (struct conn*)ev->data.ptr;
                if (ev->events & EPOLLIN) {
                    int finished = 0;
                    for (;;) {
                        ssize_t ret = read(c->fd, c->in_buf.data, buf_remains(&c->in_buf));
                        if (ret == 0) {
                            printf("conn closed\n");
                            conn_close(c);
                            finished = 1;
                            break;
                        }

                        if (ret < 0) {
                            if (errno != EAGAIN) {
                                perror("read");
                                conn_close(c);
                            }
                            break;
                        }

                        buf_advance(&c->in_buf, ret);
                    }

                    if (finished) {
                        continue;
                    }

                    buf_reset(&c->path_buf);
                    ret = parse_http_request(&c->in_buf, &c->path_buf);
                    if (ret < 0) {
                        conn_close(c);
                        printf("invalid request\n");
                    } else if (ret == 1) {
                        printf("GET %.*s\n", c->path_buf.pos, c->path_buf.data);
                        buf_reset(&c->out_buf);
                        memcpy(c->out_buf.data, "HTTP/1.1 200 Ok\r\nContent-Length: 5\r\n\r\ntest\n", 43);
                        buf_advance(&c->out_buf, 43);
                        ev_mod_fd(loop, c->fd, EPOLLOUT, c);
                    }
                } else if (ev->events & EPOLLOUT) {
                    size_t sz = c->out_buf.pos;
                    if (sz == 0) {
                        conn_close(c);
                        continue;
                    }
                    ssize_t ret = write_all(c->fd, c->out_buf.data, &sz);
                    if (ret < 0) {
                        if (ret != EAGAIN) {
                            conn_close(c);
                        }
                        continue;
                    }
                    buf_chop(&c->out_buf, ret);
                }
            }
        }
    }

    close(sock);
}

#include "WifuEndAPI.h"

string file("weapils1");
WifuEndAPILocalSocket weapils(file);

int wifu_socket(int domain, int type, int protocol) {
    weapils.wifu_socket(domain, type, protocol);
}

int wifu_bind(int fd, const struct sockaddr* addr, socklen_t len) {
    return 0;
}

int wifu_listen(int fd, int n) {
    return 0;
}

int wifu_accept(int fd, struct sockaddr* addr, socklen_t *__restrict addr_len) {
    return 0;
}

ssize_t wifu_send(int fd, const void* buf, size_t n, int flags) {
    return 0;
}

ssize_t wifu_recv(int fd, void* buf, size_t n, int flags) {
    return 0;
}

ssize_t wifu_sendto(int fd, const void* buf, size_t n, int flags, const struct sockaddr* addr, socklen_t addr_len) {
    return 0;
}

ssize_t wifu_recvfrom(int fd, void *__restrict buf, size_t n, int flags, struct sockaddr* addr,socklen_t *__restrict addr_len) {
    return 0;
}

int wifu_connect (int fd, const struct sockaddr* addr, socklen_t len) {
    return 0;
}

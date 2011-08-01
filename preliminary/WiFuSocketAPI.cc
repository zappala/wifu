#include "headers/WiFuSocketAPI.h"

gcstring WiFuSocketAPI::get_type() {
    return "wifu";
}

int WiFuSocketAPI::custom_socket(int domain, int type, int protocol) {
    return wifu_socket(domain, type, protocol);
}

int WiFuSocketAPI::custom_bind(int fd, const struct sockaddr* addr, socklen_t len) {
    return wifu_bind(fd, addr, len);
}

int WiFuSocketAPI::custom_getsockopt(int fd, int level, int optname, void *__restrict optval, socklen_t *__restrict optlen) {
    return wifu_getsockopt(fd, level, optname, optval, optlen);
}

int WiFuSocketAPI::custom_setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen) {
    return wifu_setsockopt(fd, level, optname, optval, optlen);
}

int WiFuSocketAPI::custom_listen(int fd, int n) {
    return wifu_listen(fd, n);
}

int WiFuSocketAPI::custom_accept(int fd, struct sockaddr* addr, socklen_t *__restrict addr_len) {
    return wifu_accept(fd, addr, addr_len);
}

ssize_t WiFuSocketAPI::custom_send(int fd, const void* buf, size_t n, int flags) {
    return wifu_send(fd, buf, n, flags);
}

ssize_t WiFuSocketAPI::custom_recv(int fd, void* buf, size_t n, int flags) {
    return wifu_recv(fd, buf, n, flags);
}

ssize_t WiFuSocketAPI::custom_sendto(int fd, const void* buf, size_t n, int flags, const struct sockaddr* addr, socklen_t addr_len) {
    return wifu_sendto(fd, buf, n, flags, addr, addr_len);
}

ssize_t WiFuSocketAPI::custom_recvfrom(int fd, void *__restrict buf, size_t n, int flags, struct sockaddr* addr, socklen_t *__restrict addr_len) {
    return wifu_recvfrom(fd, buf, n, flags, addr, addr_len);
}

int WiFuSocketAPI::custom_connect(int fd, const struct sockaddr* addr, socklen_t len) {
    return wifu_connect(fd, addr, len);
}

int WiFuSocketAPI::custom_close(int fd) {
    return wifu_close(fd);
}

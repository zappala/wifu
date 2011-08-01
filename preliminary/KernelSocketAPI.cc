#include "headers/KernelSocketAPI.h"

gcstring KernelSocketAPI::get_type() {
    return "kernel";
}

int KernelSocketAPI::custom_socket(int domain, int type, int protocol) {
    return socket(domain, type, protocol);
}

int KernelSocketAPI::custom_bind(int fd, const struct sockaddr* addr, socklen_t len) {
    return bind(fd, addr, len);
}

int KernelSocketAPI::custom_getsockopt(int fd, int level, int optname, void *__restrict optval, socklen_t *__restrict optlen) {
    return getsockopt(fd, level, optname, optval, optlen);
}

int KernelSocketAPI::custom_setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen) {
    return setsockopt(fd, level, optname, optval, optlen);
}

int KernelSocketAPI::custom_listen(int fd, int n) {
    return listen(fd, n);
}

int KernelSocketAPI::custom_accept(int fd, struct sockaddr* addr, socklen_t *__restrict addr_len) {
    return accept(fd, addr, addr_len);
}

ssize_t KernelSocketAPI::custom_send(int fd, const void* buf, size_t n, int flags) {
    return send(fd, buf, n, flags);
}

ssize_t KernelSocketAPI::custom_recv(int fd, void* buf, size_t n, int flags) {
    return recv(fd, buf, n, flags);
}

ssize_t KernelSocketAPI::custom_sendto(int fd, const void* buf, size_t n, int flags, const struct sockaddr* addr, socklen_t addr_len) {
    return sendto(fd, buf, n, flags, addr, addr_len);
}

ssize_t KernelSocketAPI::custom_recvfrom(int fd, void *__restrict buf, size_t n, int flags, struct sockaddr* addr, socklen_t *__restrict addr_len) {
    return recvfrom(fd, buf, n, flags, addr, addr_len);
}

int KernelSocketAPI::custom_connect(int fd, const struct sockaddr* addr, socklen_t len) {
    return connect(fd, addr, len);
}

int KernelSocketAPI::custom_close(int fd) {
    return close(fd);
}


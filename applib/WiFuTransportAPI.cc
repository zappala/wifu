#include "WiFuTransportAPI.h"


int wifu_socket(int domain, int type, int protocol) {
    return wifu_transport_local_socket.wifu_socket(domain, type, protocol);
}

int wifu_bind(int fd, const struct sockaddr* addr, socklen_t len) {
    return wifu_transport_local_socket.wifu_bind(fd, addr, len);
}

int wifu_getsockopt(int fd, int level, int optname, void *__restrict optval, socklen_t *__restrict optlen) {
    return wifu_transport_local_socket.wifu_getsockopt(fd, level, optname, optval, optlen);
}

int wifu_setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen) {
    return wifu_transport_local_socket.wifu_setsockopt(fd, level, optname, optval, optlen);
}

int wifu_listen(int fd, int n) {
    return wifu_transport_local_socket.wifu_listen(fd, n);
}

int wifu_accept(int fd, struct sockaddr* addr, socklen_t *__restrict addr_len) {
    return wifu_transport_local_socket.wifu_accept(fd, addr, addr_len);
}

ssize_t wifu_send(int fd, const void* buf, size_t n, int flags) {
    return wifu_transport_local_socket.wifu_send(fd, buf, n, flags);
}

ssize_t wifu_recv(int fd, void* buf, size_t n, int flags) {
    return wifu_transport_local_socket.wifu_recv(fd, buf, n, flags);
}

ssize_t wifu_sendto(int fd, const void* buf, size_t n, int flags, const struct sockaddr* addr, socklen_t addr_len) {
    return wifu_transport_local_socket.wifu_sendto(fd, buf, n, flags, addr, addr_len);
}

ssize_t wifu_recvfrom(int fd, void *__restrict buf, size_t n, int flags, struct sockaddr* addr,socklen_t *__restrict addr_len) {
    return wifu_transport_local_socket.wifu_recvfrom(fd, buf, n, flags, addr, addr_len);
}

int wifu_connect (int fd, const struct sockaddr* addr, socklen_t len) {
    return wifu_transport_local_socket.wifu_connect(fd, addr, len);
}

int wifu_close(int fd) {
    return wifu_transport_local_socket.wifu_close(fd);
}

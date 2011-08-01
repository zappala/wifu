/* 
 * File:   KernelSocketAPI.h
 * Author: rbuck
 *
 * Created on August 1, 2011, 3:15 PM
 */

#ifndef KERNELSOCKETAPI_H
#define	KERNELSOCKETAPI_H

#include "../applib/wifu_socket.h"
#include "ISocketAPI.h"

#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

class KernelSocketAPI : public ISocketAPI {
public:
    gcstring get_type();

    int custom_socket(int domain, int type, int protocol);

    int custom_bind(int fd, const struct sockaddr* addr, socklen_t len);

    int custom_getsockopt(int fd, int level, int optname, void *__restrict optval, socklen_t *__restrict optlen);

    int custom_setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen);

    int custom_listen(int fd, int n);

    int custom_accept(int fd, struct sockaddr* addr, socklen_t *__restrict addr_len);

    ssize_t custom_send(int fd, const void* buf, size_t n, int flags);

    ssize_t custom_recv(int fd, void* buf, size_t n, int flags);

    ssize_t custom_sendto(int fd, const void* buf, size_t n, int flags, const struct sockaddr* addr, socklen_t addr_len);

    ssize_t custom_recvfrom(int fd, void *__restrict buf, size_t n, int flags, struct sockaddr* addr, socklen_t *__restrict addr_len);

    int custom_connect(int fd, const struct sockaddr* addr, socklen_t len);

    int custom_close(int fd);
};

#endif	/* KERNELSOCKETAPI_H */


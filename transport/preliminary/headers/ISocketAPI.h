/* 
 * File:   SocketInterface.h
 * Author: rbuck
 *
 * Created on August 1, 2011, 2:46 PM
 */

#ifndef SOCKETINTERFACE_H
#define	SOCKETINTERFACE_H

#include <sys/socket.h>

#include "GarbageCollector.h"
#include "defines.h"

class ISocketAPI : public gc {
public:

    virtual gcstring get_type() = 0;

    virtual int custom_socket(int domain, int type, int protocol) = 0;

    virtual int custom_bind(int fd, const struct sockaddr* addr, socklen_t len) = 0;

    virtual int custom_getsockopt(int fd, int level, int optname, void *__restrict optval, socklen_t *__restrict optlen) = 0;

    virtual int custom_setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen) = 0;

    virtual int custom_listen(int fd, int n) = 0;

    virtual int custom_accept(int fd, struct sockaddr* addr, socklen_t *__restrict addr_len) = 0;

    virtual ssize_t custom_send(int fd, const void* buf, size_t n, int flags) = 0;

    virtual ssize_t custom_recv(int fd, void* buf, size_t n, int flags) = 0;

    virtual ssize_t custom_sendto(int fd, const void* buf, size_t n, int flags, const struct sockaddr* addr, socklen_t addr_len) = 0;

    virtual ssize_t custom_recvfrom(int fd, void *__restrict buf, size_t n, int flags, struct sockaddr* addr, socklen_t *__restrict addr_len) = 0;

    virtual int custom_connect(int fd, const struct sockaddr* addr, socklen_t len) = 0;

    virtual int custom_close(int fd) = 0;
};

#endif	/* SOCKETINTERFACE_H */


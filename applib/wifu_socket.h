/* 
 * File:   wifu_socket.h
 * Author: rbuck
 *
 * Created on November 26, 2010, 2:16 PM
 */

#ifndef WIFU_SOCKET_H
#define	WIFU_SOCKET_H

#include <stdlib.h>
#include <sys/socket.h>

/**
 * @see man 2 socket
 */
int wifu_socket(int domain, int type, int protocol);

/**
 * @see man 2 bind
 */
int wifu_bind(int fd, const struct sockaddr* addr, socklen_t len);

/**
 * @see man 2 getsockopt
 */
int wifu_getsockopt (int fd, int level, int optname, void *__restrict optval, socklen_t *__restrict optlen);

/**
 * @see man 2 setsockopt
 */
int wifu_setsockopt (int fd, int level, int optname, const void *optval, socklen_t optlen);

/**
 * @return 0 on success, -1 for errors.
 */
int wifu_listen(int fd, int n);

/**
 * @see man 2 accept
 */
int wifu_accept(int fd, struct sockaddr* addr, socklen_t *__restrict addr_len);

/**
 * @see man 2 send
 */
ssize_t wifu_send(int fd, const void* buf, size_t n, int flags);

/**.
 * @see man 2 recv
 */
ssize_t wifu_recv(int fd, void* buf, size_t n, int flags);

/**
 * @see man 2 sendto
 */
ssize_t wifu_sendto(int fd, const void* buf, size_t n, int flags, const struct sockaddr* addr, socklen_t addr_len);

/**
 * @see man 2 recvfrom
 */
ssize_t wifu_recvfrom(int fd, void *__restrict buf, size_t n, int flags, struct sockaddr* addr,socklen_t *__restrict addr_len);

/**
 * @see man 2 connect
 */
int wifu_connect (int fd, const struct sockaddr* addr, socklen_t len);

/**
 * @see man 2 close
 */
int wifu_close(int fd);


#endif	/* WIFU_SOCKET_H */


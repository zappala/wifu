/* 
 * File:   WifuEndAPI.h
 * Author: rbuck
 *
 * Created on November 19, 2010, 3:39 PM
 */

#ifndef _WIFUENDAPI_H
#define	_WIFUENDAPI_H

#include <unistd.h>


/* Create a new socket of type TYPE in domain DOMAIN, using
   protocol PROTOCOL.  If PROTOCOL is zero, one is chosen automatically.
   Returns a file descriptor for the new socket, or -1 for errors.  */
int wifu_socket(int domain, int type, int protocol);

/* Give the socket FD the local address ADDR (which is LEN bytes long).  */
int wifu_bind(int fd, const struct sockaddr* addr, socklen_t len);

/* Prepare to accept connections on socket FD.
   N connection requests will be queued before further requests are refused.
   Returns 0 on success, -1 for errors.  */
int wifu_listen(int fd, int n);

/* Await a connection on socket FD.
   When a connection arrives, open a new socket to communicate with it,
   set *ADDR (which is *ADDR_LEN bytes long) to the address of the connecting
   peer and *ADDR_LEN to the address's actual length, and return the
   new socket's descriptor, or -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
int wifu_accept(int fd, struct sockaddr* addr, socklen_t *__restrict addr_len);


/* Send N bytes of BUF to socket FD.  Returns the number sent or -1.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
ssize_t wifu_send(int fd, const void* buf, size_t n, int flags);

/* Read N bytes into BUF from socket FD.
   Returns the number read or -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
ssize_t wifu_recv(int fd, void* buf, size_t n, int flags);

/* Send N bytes of BUF on socket FD to peer at address ADDR (which is
   ADDR_LEN bytes long).  Returns the number sent, or -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
ssize_t wifu_sendto(int fd, const void* buf, size_t n, int flags, const struct sockaddr* addr, socklen_t addr_len);

/* Read N bytes into BUF through socket FD.
   If ADDR is not NULL, fill in *ADDR_LEN bytes of it with tha address of
   the sender, and store the actual size of the address in *ADDR_LEN.
   Returns the number of bytes read or -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
ssize_t wifu_recvfrom(int fd, void *__restrict buf, size_t n, int flags, struct sockaddr* addr,socklen_t *__restrict addr_len);

/* Open a connection on socket FD to peer at ADDR (which LEN bytes long).
   For connectionless socket types, just set the default address to send to
   and the only address from which to accept transmissions.
   Return 0 on success, -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
int wifu_connect (int fd, const struct sockaddr* addr, socklen_t len);

//Server socket calls that we will need :
//socket() creates a socket
//bind() binds an address(and port ?) to a socket.
//listen() listens on a socket for incoming connections.
//accept() accepts connections on a socket, stores the client address / port. // blocks
//recv() receive from a socket and put it into a buffer(socket must be connected) //blocks
//recvfrom() same as recv() but also saves the address of the src of the data received //blocks
//send() send a message to another socket
//sendto() same as send, but sends to an address. If the socket is in connected mode the address is ignored and data is sent to the socket.
//
//We can likely only use send() and recv() because we will be dealing with connection oriented protocols. What if we want to use UDP ?
//
//Client socket calls that we will need :
//socket()
//connect() connects socket to an address / port(server) // blocks
//recv() // blocks
//recvfrom() //blocks
//send()
//sendto()

#endif /* _WIFUENDAPI_H */


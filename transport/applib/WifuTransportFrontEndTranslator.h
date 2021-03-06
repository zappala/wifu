/* 
 * File:   WifuTransportFrontEndTranslator.h
 * Author: rbuck
 *
 * Created on November 22, 2010, 3:10 PM
 */

#ifndef _WIFUTRANSPORTFRONTENDTRANSLATOR_H
#define	_WIFUTRANSPORTFRONTENDTRANSLATOR_H



#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <list>

#include "QueryStringParser.h"
#include "LocalSocketFullDuplex.h"
#include "LocalSocketReceiverCallback.h"
#include "BinarySemaphore.h"
#include "Utils.h"
#include "AddressPort.h"
#include "defines.h"
#include "IDGenerator.h"
#include "SocketDataMap.h"
#include "SocketData.h"
#include "ObjectPool.h"

#include "MessageStructDefinitions.h"

#define sockets SocketDataMap::instance()


// TODO: Go over each man page and determine what we want to support,
// TODO: then make sure that every function in this file supports that behavior.

/**
 * Communicates (sends messages) with the back-end over a Unix Socket, writing to the file /tmp/WS.
 * Receives messages from the back-end over a Unix Socket.  This class receives on the file /tmp/LS plus a random number.
 *
 * Note that all these functions are simply an interface.
 * I have done the best I can with the time I've had to implement as much of this functionality as possible.
 * In a lot of cases there is not a clear delineation between what WiFu Transport should support versus what is left to a protocol to implement.
 * This means that you may not get identical functionality as explained by the man pages.
 * However, I have consulted them as I have built this software and provide a good reference for what to expect.
 *
 */
class WifuTransportFrontEndTranslator : public LocalSocketFullDuplex {
private:

    /**
     * Constructs a WiFuTransportAPILocalSocket for use in communicating with the WiFu Transport daemon.
     *
     * @param file The file which this object listens on (other local sockets can write to this file).
     */
    WifuTransportFrontEndTranslator() : LocalSocketFullDuplex(get_filename().c_str()), write_file_("/tmp/WS") {
        socket_signal_.init(0);
        socket_mutex_.init(1);

        memset(&back_end_, 0, sizeof (struct sockaddr_un));
        back_end_.sun_family = AF_LOCAL;
        strcpy(back_end_.sun_path, write_file_.c_str());

        // ensure objects are created ahead of time
        ObjectPool<SocketData>::instance();
    }

    /**
     * Copy constructor.  Should never be called.
     * @param other The WiFuTransportAPILocalSocket to copy.
     */
    WifuTransportFrontEndTranslator(WifuTransportFrontEndTranslator const& other) : LocalSocketFullDuplex(get_file()), write_file_("/tmp/WS") {
        assert(false);
    }

    /**
     * Assignment operator.  Should never be called.
     * @param other The WiFuTransportAPILocalSocket to copy.
     */
    WifuTransportFrontEndTranslator & operator=(WifuTransportFrontEndTranslator const&) {
        assert(false);
    }

    /**
     * @return A filename that this WiFuTransportFrontEndTranslator will listen on for messages from the back-end.
     * The file will be of the format /tmp/LS plus a random number.  For example /tmp/LS123456789.
     */
    gcstring get_filename() {
        gcstring s("/tmp/LS");
        int id = IDGenerator::instance().get();
        s.append(Utils::itoa(id));
        return s;
    }


public:

    /**
     * @return Static instance of this WiFuTransportAPILocalSocket.
     */
    static WifuTransportFrontEndTranslator& instance() {
        static WifuTransportFrontEndTranslator instance_;
        return instance_;
    }

    /**
     * Destructor
     */
    virtual ~WifuTransportFrontEndTranslator() {
       
    }

    /**
     * Callback function used to receive messages sent by the back end.
     * 
     * @param message The message received.  It must be of type GenericResponseMessage (it should be a more specific message).
     * @param length Length of message in bytes.
     */
    void receive(unsigned char* message, int length) {
        struct GenericResponseMessage* response = (struct GenericResponseMessage*) message;

        if (response->message_type == WIFU_SOCKET) {
            sockets.get(0)->set_receive_payload(message, length);
            socket_signal_.post();
            return;
        }

        SocketData* data = sockets.get(response->fd);

        if (!data && response->message_type == WIFU_CLOSE) {
            // We already closed
            return;
        }

        if (!data) {
            //TODO: is this really an error?
            assert(data);
            return;
        }

        data->get_flag()->wait();
        data->set_receive_payload(message, length);
        data->get_semaphore()->post();
    }

    /**
     * Non-blocking.
     * Creates a SocketMessage and sends it to the back end.
     * Then waits for a response.
     * See man 2 socket.
     *
     * @param domain Communication domain (usually AF_INET).
     * @param type Specifies the communication semantics (usually SOCK_STREAM).
     * @param protocol Specifies the protocol to use (TCP, TCP-AP, etc.).
     *
     * @return The new socket id.
     */
    int wifu_socket(int domain, int type, int protocol) {
        socket_mutex_.wait();

        SocketData* d = ObjectPool<SocketData>::instance().get();
        sockets.put(0, d);

        struct SocketMessage* socket_message = (struct SocketMessage*) d->get_send_payload();
        socket_message->message_type = WIFU_SOCKET;
        socket_message->length = sizeof (struct SocketMessage);
        memcpy(&(socket_message->source), get_address(), sizeof (struct sockaddr_un));

        // Put in a bad fd so it will not be found on the back end
        socket_message->fd = 0;
        socket_message->domain = domain;
        socket_message->type = type;
        socket_message->protocol = protocol;

        send_to(&back_end_, socket_message, socket_message->length);

        socket_signal_.wait();

        // TODO: Ensure that we never receive a socket id of 0        
        SocketData* data = sockets.get(0);
        assert(data);

        struct SocketResponseMessage* socket_response = (struct SocketResponseMessage*) data->get_receive_payload();
        int socket = socket_response->return_value;
        sockets.erase_at(0);
        sockets.put(socket, data);

        socket_mutex_.post();
        return socket;
    }

    /**
     * Non-blocking.
     * Bind a name to a socket
     * See man 2 bind.
     *
     * Creates a BindMessage and passes it to the back end, then waits for a response on the specified fd.
     *
     * @param fd The socket id.
     * @param addr Address to bind to.  This must be a sockaddr_in object.
     * @param len Length of object pointed to by addr.
     *
     * @return 0 if call was successfull, -1 otherwise (and ERRNO is set appropriately).
     */
    int wifu_bind(int fd, const struct sockaddr* addr, socklen_t len) {
        assert(sizeof (struct sockaddr_in) == len);

        //TODO: determine if this is the best way to check for bad fd
        // we are essentially keeping track of the fd in two places now
        // we could fix this by having a different method on the front end.
        if (sockets.get(fd) == NULL) {
            errno = EBADF;
            return -1;
        }

        SocketData* data = sockets.get(fd);

        struct BindMessage* bind_message = reinterpret_cast<struct BindMessage*> (data->get_send_payload());
        bind_message->message_type = WIFU_BIND;
        bind_message->length = sizeof (struct BindMessage);
        memcpy(&(bind_message->source), get_address(), sizeof (struct sockaddr_un));

        bind_message->fd = fd;
        memcpy(&(bind_message->addr), addr, len);
        bind_message->len = len;

        send_to(&back_end_, bind_message, bind_message->length);

        data->get_semaphore()->wait();

        struct BindResponseMessage* bind_response = (struct BindResponseMessage*) data->get_receive_payload();
        if (bind_response->error) {
            errno = bind_response->error;
        }
        int return_value = bind_response->return_value;
        data->get_flag()->post();

        return return_value;
    }

    /**
     * Creates and sends a GetSockOptMessage to the back end, then wait for a response (on a per-socket basis).
     * See man 2 getsockopt.
     *
     * @param fd The socket id.
     * @param level Should be set to the protocol number of TCP.
     * @param optname Name of option.
     * @param optval Value of the option.
     * @param optlen Length of optval.
     *
     * @return 0 if call was successfull, -1 otherwise (and ERRNO is set appropriately).
     */
    int wifu_getsockopt(int fd, int level, int optname, void *__restrict optval, socklen_t *__restrict optlen) {

        SocketData* data = sockets.get(fd);

        struct GetSockOptMessage* getsockopt_message = reinterpret_cast<struct GetSockOptMessage*> (data->get_send_payload());
        getsockopt_message->message_type = WIFU_GETSOCKOPT;
        getsockopt_message->length = sizeof (struct GetSockOptMessage);
        memcpy(&(getsockopt_message->source), get_address(), sizeof (struct sockaddr_un));

        getsockopt_message->fd = fd;
        getsockopt_message->level = level;
        getsockopt_message->optname = optname;
        getsockopt_message->optlen = *optlen;

        send_to(&back_end_, getsockopt_message, getsockopt_message->length);

        data->get_semaphore()->wait();

        struct GetSockOptResponseMessage* getsockopt_response = (struct GetSockOptResponseMessage*) data->get_receive_payload();

        socklen_t len = getsockopt_response->optlen;
        if (len > 0) {
            memcpy(optval, getsockopt_response + 1, len);
            memcpy(optlen, &len, sizeof (socklen_t));
        }

        int return_value = getsockopt_response->return_value;
        data->get_flag()->post();
        return return_value;
    }

    /**
     * Creates and sends a SetSockOptMessage to the back end, then wait for a response (on a per-socket basis).
     * See man 2 setsockopt.
     *
     * @param fd The socket id.
     * @param level Should be set to the protocol number of TCP.
     * @param optname Name of option.
     * @param optval Pointer to place to store the value.
     * @param optlen Length of optval.
     *
     * @return 0 if call was successfull, -1 otherwise (and ERRNO is set appropriately).
     */
    int wifu_setsockopt(int fd, int level, int optname, const void* optval, socklen_t optlen) {
        assert(optlen < BUFFER_SIZE);

        SocketData* data = sockets.get(fd);

        struct SetSockOptMessage* setsockopt_message = reinterpret_cast<struct SetSockOptMessage*> (data->get_send_payload());
        setsockopt_message->message_type = WIFU_SETSOCKOPT;
        setsockopt_message->length = sizeof (struct SetSockOptMessage) +optlen;
        memcpy(&(setsockopt_message->source), get_address(), sizeof (struct sockaddr_un));

        setsockopt_message->fd = fd;
        setsockopt_message->level = level;
        setsockopt_message->optname = optname;
        setsockopt_message->optlen = optlen;
        // struct ptr + 1 increases the pointer by one size of the struct
        memcpy(setsockopt_message + 1, optval, optlen);

        send_to(&back_end_, setsockopt_message, setsockopt_message->length);

        data->get_semaphore()->wait();

        struct SetSockOptResponseMessage* setsockopt_response = (struct SetSockOptResponseMessage*) data->get_receive_payload();

        int return_value = setsockopt_response->return_value;
        data->get_flag()->post();
        return return_value;
    }

    /**
     * Non-blocking.
     * Mark this socket as listening.
     * See man 2 listen.
     *
     * Creates a ListenMessage and passes it to the back end, then waits for a response on the specified fd.
     *
     * @param fd The socket id.
     * @param n The maximum number of connections to which the queue of pending connections for fd may grow.
     *
     * @return 0 if call was successfull, -1 otherwise (and ERRNO is set appropriately).
     */
    int wifu_listen(int fd, int n) {

        //TODO: determine if this is the best way to check for bad fd
        // we are essentially keeping track of the fd in two places now
        // we could fix this by having a different method on the front end.
        if (sockets.get(fd) == NULL) {
            errno = EBADF;
            return -1;
        }

        SocketData* data = sockets.get(fd);

        struct ListenMessage* listen_message = reinterpret_cast<struct ListenMessage*> (data->get_send_payload());
        listen_message->message_type = WIFU_LISTEN;
        listen_message->length = sizeof (struct ListenMessage);
        memcpy(&(listen_message->source), get_address(), sizeof (struct sockaddr_un));

        listen_message->fd = fd;
        listen_message->n = n;

        send_to(&back_end_, listen_message, listen_message->length);

        data->get_semaphore()->wait();

        struct ListenResponseMessage* listen_response = (struct ListenResponseMessage*) data->get_receive_payload();
        if (listen_response->error) {
            errno = listen_response->error;
        }
        int return_value = listen_response->return_value;
        data->get_flag()->post();
        return return_value;
    }

    /**
     * Blocking.
     * Accept incomming connections.
     * See man 2 accept.
     *
     * Creates a AcceptMessage and passes it to the back end, then waits for a response on the specified fd.
     *
     * @param fd The socket id.
     * @param addr Pointer to a structure to fill in with the remote address/port to which we connect.
     * @param addr_len the length of addr.
     *
     * @return 0 if call was successfull, -1 otherwise (and ERRNO is set appropriately).
     */
    int wifu_accept(int fd, struct sockaddr* addr, socklen_t *__restrict addr_len) {

        SocketData* data = sockets.get(fd);

        struct AcceptMessage* accept_message = reinterpret_cast<struct AcceptMessage*> (data->get_send_payload());
        accept_message->message_type = WIFU_ACCEPT;
        accept_message->length = sizeof (struct AcceptMessage);
        memcpy(&(accept_message->source), get_address(), sizeof (struct sockaddr_un));

        accept_message->fd = fd;

        if (addr != NULL && addr_len != NULL) {
            memcpy(&(accept_message->addr), addr, *addr_len);
            accept_message->len = *addr_len;
        } else {
            accept_message->len = 0;
        }

        send_to(&back_end_, accept_message, accept_message->length);

        data->get_semaphore()->wait();

        struct AcceptResponseMessage* accept_response = (struct AcceptResponseMessage*) data->get_receive_payload();

        if (addr != NULL && addr_len != NULL) {
            memcpy(addr_len, &(accept_response->addr_len), sizeof (socklen_t));
            memcpy(addr, &(accept_response->addr), (size_t) accept_response->addr_len);
        }

        int new_socket = accept_response->return_value;
        sockets.put(new_socket, ObjectPool<SocketData>::instance().get());
        data->get_flag()->post();
        return new_socket;
    }

    /**
     * Possibly Blocking.
     * Send data to a remote destination.
     * See man 2 send.
     *
     * Creates a SendToMessage and passes it to the back end, then waits for a response on the specified fd.
     *
     * @param fd The socket id.
     * @param buf The buffer to send.
     * @param n The length of buf.
     * @param flags See man 2 send.
     *
     * @return On success, the number of characters sent.  If an error occurs, -1 is returned and ERRNO is set.
     */
    ssize_t wifu_send(int fd, const void* buf, size_t n, int flags) {
        return wifu_sendto(fd, buf, n, flags, 0, 0);
    }

    /**
     * Blocking.
     * Receive data on a socket.
     * See man 2 send.
     *
     * Creates a RecvFromMessage and passes it to the back end, then waits for a response on the specified fd.
     *
     * @param fd The socket id to receive on.
     * @param buf The buffer to fill upon recept of data.
     * @param n The length of buf.
     * @param flags See man 2 recv.
     *
     * @return On success, the number of bytes received.  If an error occurs, -1 is returned and ERRNO is set.
     * The return value may also be 0 if the peer performed an orderly shutdown.
     */
    ssize_t wifu_recv(int fd, void* buf, size_t n, int flags) {
        return wifu_recvfrom(fd, buf, n, flags, 0, 0);
    }

    /**
     * Possibly Blocking.
     * Send data to a remote destination.
     * See man 2 send.
     *
     * Creates a SendToMessage and passes it to the back end, then waits for a response on the specified fd.
     *
     * @param fd The socket id.
     * @param buf The buffer to send.
     * @param n The length of buf.
     * @param flags See man 2 send.
     * @param addr The address to send to.
     * @param addr_len The length of addr.
     *
     * @return On success, the number of characters sent.  If an error occurs, -1 is returned and ERRNO is set.
     */
    ssize_t wifu_sendto(int fd, const void* buf, size_t n, int flags, const struct sockaddr* addr, socklen_t addr_len) {

        SocketData* data = sockets.get(fd);

        struct SendToMessage* sendto_message = reinterpret_cast<struct SendToMessage*> (data->get_send_payload());
        sendto_message->message_type = WIFU_SENDTO;
        sendto_message->length = sizeof (struct SendToMessage) +n;
        memcpy(&(sendto_message->source), get_address(), sizeof (struct sockaddr_un));

        sendto_message->fd = fd;
        if (addr != 0 && addr_len != 0) {
            memcpy(&(sendto_message->addr), addr, addr_len);
            sendto_message->len = addr_len;
        } else {
            sendto_message->len = 0;
        }
        sendto_message->buffer_length = n;
        sendto_message->flags = flags;
        memcpy(sendto_message + 1, buf, n);

        send_to(&back_end_, sendto_message, sendto_message->length);

        data->get_semaphore()->wait();

        struct SendToResponseMessage* sendto_response = (struct SendToResponseMessage*) data->get_receive_payload();
        int return_value = sendto_response->return_value;
        data->get_flag()->post();
        return return_value;
    }

    /**
     * Blocking.
     * Receive data on a socket.
     * See man 2 send.
     *
     * Creates a RecvFromMessage and passes it to the back end, then waits for a response on the specified fd.
     *
     * @param fd The socket id to receive on.
     * @param buf The buffer to fill upon recept of data.
     * @param n The length of buf.
     * @param flags See man 2 recv.
     * @param addr The address to receive from.
     * @param addr_len the length of addr.
     *
     * @return On success, the number of bytes received.  If an error occurs, -1 is returned and ERRNO is set.
     * The return value may also be 0 if the peer performed an orderly shutdown.
     */
    ssize_t wifu_recvfrom(int fd, void *__restrict buf, size_t n, int flags, struct sockaddr* addr, socklen_t *__restrict addr_len) {
        SocketData* data = sockets.get(fd);

        struct RecvFromMessage* recvfrom_message = reinterpret_cast<struct RecvFromMessage*> (data->get_send_payload());
        recvfrom_message->message_type = WIFU_RECVFROM;
        recvfrom_message->length = sizeof (struct RecvFromMessage);
        memcpy(&(recvfrom_message->source), get_address(), sizeof (struct sockaddr_un));

        recvfrom_message->fd = fd;
        if (addr != 0 && addr_len != 0) {
            memcpy(&(recvfrom_message->addr), addr, *addr_len);
            recvfrom_message->len = *addr_len;
        } else {
            recvfrom_message->len = 0;
        }
        recvfrom_message->buffer_length = n;
        recvfrom_message->flags = flags;

        send_to(&back_end_, recvfrom_message, recvfrom_message->length);

        assert(data != NULL);
        assert(data->get_semaphore() != NULL);

        data->get_semaphore()->wait();

        struct GenericResponseMessage* response = (struct GenericResponseMessage*) data->get_receive_payload();
        int ret_val = response->return_value;

        if (response->message_type == WIFU_RECVFROM) {
            struct RecvFromResponseMessage* recvfrom_response = (struct RecvFromResponseMessage*) data->get_receive_payload();
            if (ret_val > 0) {
                memcpy(buf, recvfrom_response + 1, ret_val);
                if (addr != 0 && addr_len != 0) {
                    memcpy(addr_len, &recvfrom_response->addr_len, sizeof (socklen_t));
                    memcpy(addr, &recvfrom_response->addr, *addr_len);
                }
            }
        }

        data->get_flag()->post();
        return ret_val;
    }

    /**
     * Non-blocking.
     * Connect to a remote peer.
     * See man 2 connect.
     *
     * Creates a ConnectMessage and passes it to the back end, then waits for a response on the specified fd.
     *
     * @param fd The socket id.
     * @param addr The address/port of the remote peer to connect to.
     * @param len the length of addr.
     *
     * @return 0 if call was successfull, -1 otherwise (and ERRNO is set appropriately).
     */
    int wifu_connect(int fd, const struct sockaddr* addr, socklen_t len) {
        assert(addr != NULL);
        assert(sizeof (struct sockaddr_in) == len);

        SocketData* data = sockets.get(fd);

        struct ConnectMessage* connect_message = reinterpret_cast<struct ConnectMessage*> (data->get_send_payload());
        connect_message->message_type = WIFU_CONNECT;
        connect_message->length = sizeof (struct ConnectMessage);
        memcpy(&(connect_message->source), get_address(), sizeof (struct sockaddr_un));

        connect_message->fd = fd;
        memcpy(&(connect_message->addr), addr, len);
        connect_message->len = len;

        send_to(&back_end_, connect_message, connect_message->length);

        data->get_semaphore()->wait();

        struct ConnectResponseMessage* connect_response = (struct ConnectResponseMessage*) data->get_receive_payload();
        int return_value = connect_response->return_value;
        data->get_flag()->post();
        return return_value;
    }

    /**
     * Closes fd and removes any notion that it existed.
     * Sends a CloseMessage to the back end and waits for a response.
     * @param fd The file descriptor to close.
     * @return 0 if call was successfull, -1 otherwise (and ERRNO is set appropriately).
     */
    int wifu_close(int fd) {

        SocketData* data = sockets.get(fd);

        struct CloseMessage* connect_message = reinterpret_cast<struct CloseMessage*> (data->get_send_payload());
        connect_message->message_type = WIFU_CLOSE;
        connect_message->length = sizeof (struct CloseMessage);
        memcpy(&(connect_message->source), get_address(), sizeof (struct sockaddr_un));

        connect_message->fd = fd;

        send_to(&back_end_, connect_message, connect_message->length);

        data->get_semaphore()->wait();

        struct CloseResponseMessage* close_response = (struct CloseResponseMessage*) data->get_receive_payload();
        int return_value = close_response->return_value;

        data->get_flag()->post();

        ObjectPool<SocketData>::instance().release(data);

        sockets.erase_at(fd);
        return return_value;
    }

private:
    /**
     * The file this WiFuTransportFrontEndTranslator will write to in order to send messages to the back-end (/tmp/WS).
     */
    gcstring write_file_;

    /**
     * The back-end address (Unix socket)
     */
    struct sockaddr_un back_end_;

    /**
     * Special Semaphore used to indicate we are sending/receiving a wifu_socket message.
     */
    BinarySemaphore socket_signal_;

    /**
     * Semaphore to only allow one call to wifu_socket at a time.
     */
    BinarySemaphore socket_mutex_;
};

#endif	/* _WIFUTRANSPORTFRONTENDTRANSLATOR_H */



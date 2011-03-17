/* 
 * File:   WifuEndAPILocalSocket.h
 * Author: rbuck
 *
 * Created on November 22, 2010, 3:10 PM
 */

#ifndef _WIFUENDAPILOCALSOCKET_H
#define	_WIFUENDAPILOCALSOCKET_H



#include <sstream>
#include <assert.h>

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


#define sockets SocketDataMap::instance()

// TODO: Go over each man page and determine what we want to support,
// TODO: then make sure that every function in this file supports that behavior.

/**
 * Communicates (sends messages) with the back-end over a Unix Socket, writing to the file /tmp/WS.
 * Receives messages from the back-end over a Unix Socket.  This class receives on the file /tmp/LS plus a random number.
 * Messages are of the format: method_name?key0=value0&key1=value1&
 *
 */
class WifuEndAPILocalSocket : public LocalSocketFullDuplex {
private:

    /**
     * Constructs a WifuEndAPILocalSocket for use in communicating with the Wifu End process
     *
     * @param file The file which this object listens on (other local sockets can write to this file).
     */
    WifuEndAPILocalSocket() : LocalSocketFullDuplex(get_filename().c_str()), write_file_("/tmp/WS") {
        socket_signal_.init(0);
        socket_mutex_.init(1);

    }

    /**
     * Copy constructor.  Should never be called.
     * @param other The WifuEndAPILocalSocket to copy.
     */
    WifuEndAPILocalSocket(WifuEndAPILocalSocket const& other) : LocalSocketFullDuplex(get_file()), write_file_("/tmp/WS") {
        assert(false);
    }

    /**
     * Assignment operator.  Should never be called.
     * @param other The WifuEndAPILocalSocket to copy.
     */
    WifuEndAPILocalSocket & operator=(WifuEndAPILocalSocket const&) {
        assert(false);
    }

    /**
     * @return A filename that is WifuEndAPILocalSocket will listen on for messages from the back-end.
     * The file will be of the format /tmp/LS plus a random number.  For example /tmp/LS123456789
     */
    string get_filename() {
        string s("/tmp/LS");
        int id = IDGenerator::instance().get();
        s.append(Utils::itoa(id));
        return s;
    }


public:

    /**
     * @return Static instance of this WifuEndAPILocalSocket
     */
    static WifuEndAPILocalSocket& instance() {
        static WifuEndAPILocalSocket instance_;
        return instance_;
    }

    /**
     * Destructor
     */
    virtual ~WifuEndAPILocalSocket() {

    }

    /**
     * This is the callback function where messages received come
     * This function fills in the appropriate fields in the SocketData object
     * associated with the socket id.
     * Finally it posts on the Semaphore internal to to the above mentioned SocketData object.
     *
     * @param message The message received from the back-end
     *
     * @see SocketData
     */
    void receive(string& message) {
        //        cout << "Response:\t" << message << endl;
        response_.clear();
        QueryStringParser::parse(message, response_);
        int socket = atoi(response_[SOCKET_STRING].c_str());


        if (!response_[NAME_STRING].compare(WIFU_SOCKET_NAME)) {
            sockets.put(0, new SocketData());
            sockets.get(0)->set_return_value(socket);
            socket_signal_.post();
            return;
        }

        if (!response_[NAME_STRING].compare(WIFU_RECVFROM_NAME)) {
//            cout << "WifuEndAPILocalSocket::receive(): " << response_[BUFFER_STRING];
            sockets.get(socket)->set_payload(response_[BUFFER_STRING]);
        }

        if (!response_[NAME_STRING].compare(WIFU_GETSOCKOPT_NAME)) {
            string response = response_[BUFFER_STRING];
            sockets.get(socket)->set_payload(response);
            sockets.get(socket)->set_payload_length(response.size());
        }

        if (!response_[NAME_STRING].compare(WIFU_ACCEPT_NAME)) {
            string address = response_[ADDRESS_STRING];
            u_int16_t port = atoi(response_[PORT_STRING].c_str());
            AddressPort* ap = new AddressPort(address, port);
            sockets.get(socket)->set_address_port(ap);
        }

        int value = atoi(response_[RETURN_VALUE_STRING].c_str());
        int error = atoi(response_[ERRNO].c_str());

        SocketData* data = sockets.get(socket);
        assert(data);

        data->set_error(error);
        data->set_return_value(value);
        data->get_semaphore()->post();
    }

    /**
     * Non-blocking
     * Creates a socket message and sends it to the back end.
     * Then waits for a response
     * See man 2 socket
     *
     * @param domain Communication domain (usually AF_INET).
     * @param type Specifies the communication semantics (usually SOCK_STREAM).
     * @param protocol Specifies the protocol to use (TCP, TCP-AP, etc.).
     *
     * @return The new socket id
     */
    int wifu_socket(int domain, int type, int protocol) {
        socket_mutex_.wait();
        map<string, string> m;
        m[FILE_STRING] = get_file();
        m[DOMAIN_STRING] = Utils::itoa(domain);
        m[TYPE_STRING] = Utils::itoa(type);
        m[PROTOCOL_STRING] = Utils::itoa(protocol);
        string message = QueryStringParser::create(WIFU_SOCKET_NAME, m);
        send_to(write_file_, message);

        socket_signal_.wait();

        // TODO: Ensure that we never receive a socket id of 0        
        SocketData* data = sockets.get(0);
        assert(data);
        int socket = data->get_return_value();
        sockets.erase_at(0);
        sockets.put(socket, data);

        socket_mutex_.post();
        return socket;
    }

    /**
     * Non-blocking
     * Bind a name to a socket
     * See man 2 bind
     *
     * Creates a wifu_bind message and passes it to the back-end, then waits for a response on the specified fd.
     *
     * @param fd The socket id
     * @param addr Address to bind to.  This must be a sockaddr_in object.
     * @param len Length of object pointed to by addr.
     *
     * @return 0 if call was successfull, -1 otherwise (and ERRNO is set appropriately)
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

        AddressPort ap((struct sockaddr_in*) addr);

        map<string, string> m;
        m[FILE_STRING] = get_file();
        m[SOCKET_STRING] = Utils::itoa(fd);
        m[ADDRESS_STRING] = ap.get_address();
        m[PORT_STRING] = Utils::itoa(ap.get_port());
        m[LENGTH_STRING] = Utils::itoa(len);

        string message = QueryStringParser::create(WIFU_BIND_NAME, m);
        send_to(write_file_, message);

        SocketData* data = sockets.get(fd);
        data->get_semaphore()->wait();

        int error = data->get_error();
        if (error) {
            errno = error;
        }

        return data->get_return_value();
    }

    /**
     * Creates and sends a getsockopt message to the back-end, then wait for a response (on a per-socket basis).
     * See man 2 getsockopt
     *
     * @param fd The socket id
     * @param level Should be set to the protocol number of TCP
     * @param optname Name of option
     * @param optval Value of the option
     * @param optlen Length of optval
     *
     * @return 0 if call was successfull, -1 otherwise (and ERRNO is set appropriately)
     */
    int wifu_getsockopt(int fd, int level, int optname, void *__restrict optval, socklen_t *__restrict optlen) {

        map<string, string> m;
        m[FILE_STRING] = get_file();
        m[SOCKET_STRING] = Utils::itoa(fd);
        m[LEVEL_STRING] = Utils::itoa(level);
        m[OPTION_NAME_STRING] = Utils::itoa(optname);
        m[LENGTH_STRING] = Utils::itoa(*optlen);

        string message = QueryStringParser::create(WIFU_GETSOCKOPT_NAME, m);
        send_to(write_file_, message);

        //TODO: Fill in optval and optlen according to man 2 getsockopt
        SocketData* data = sockets.get(fd);
        data->get_semaphore()->wait();

        socklen_t len = data->get_payload_length();
        if (len > 0) {
            memcpy(optval, data->get_payload(), len);
            memcpy(optlen, &len, sizeof (socklen_t));
        }

        return data->get_return_value();
    }

    /**
     * Creates and sends a wetsockopt message to the back-end, then wait for a response (on a per-socket basis).
     * See man 2 setsockopt
     *
     * @param fd The socket id
     * @param level Should be set to the protocol number of TCP
     * @param optname Name of option
     * @param optval Pointer to place to store the value
     * @param optlen Length of optval
     *
     * @return 0 if call was successfull, -1 otherwise (and ERRNO is set appropriately)
     */
    int wifu_setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen) {

        assert(optlen < BUFFER_SIZE);
        char value[BUFFER_SIZE];
        memcpy(value, optval, optlen);

        map<string, string> m;
        m[FILE_STRING] = get_file();
        m[SOCKET_STRING] = Utils::itoa(fd);
        m[LEVEL_STRING] = Utils::itoa(level);
        m[OPTION_NAME_STRING] = Utils::itoa(optname);
        m[OPTION_VALUE_STRING] = value;
        m[LENGTH_STRING] = Utils::itoa(optlen);

        string message = QueryStringParser::create(WIFU_SETSOCKOPT_NAME, m);
        send_to(write_file_, message);

        SocketData* data = sockets.get(fd);
        data->get_semaphore()->wait();

        return data->get_return_value();
    }

    /**
     * Non-blocking
     * Mark this socket as listening
     * See man 2 listen
     *
     * Creates a wifu_listen message and passes it to the back-end, then waits for a response on the specified fd.
     *
     * @param fd The socket id
     * @param n The maximum number of connections to which the queue of pending connections for fd may grow.
     *
     * @return 0 if call was successfull, -1 otherwise (and ERRNO is set appropriately)
     */
    int wifu_listen(int fd, int n) {

        //TODO: determine if this is the best way to check for bad fd
        // we are essentially keeping track of the fd in two places now
        // we could fix this by having a different method on the front end.
        if (sockets.get(fd) == NULL) {
            errno = EBADF;
            return -1;
        }


        map<string, string> m;
        m[FILE_STRING] = get_file();
        m[SOCKET_STRING] = Utils::itoa(fd);
        m[N_STRING] = Utils::itoa(n);

        string message = QueryStringParser::create(WIFU_LISTEN_NAME, m);
        send_to(write_file_, message);

        SocketData* data = sockets.get(fd);
        data->get_semaphore()->wait();

        int error = data->get_error();
        if (error) {
            errno = error;
        }

        return data->get_return_value();
    }

    /**
     * Blocking
     * Accept incomming connections
     * See man 2 accept
     *
     * Creates a wifu_accept message and passes it to the back-end, then waits for a response on the specified fd.
     *
     * @param fd The socket id
     * @param addr Pointer to a structure to fill in with the remote address/port to which we connect
     * @param addr_len the length of addr
     *
     * @return 0 if call was successfull, -1 otherwise (and ERRNO is set appropriately)
     */
    int wifu_accept(int fd, struct sockaddr* addr, socklen_t *__restrict addr_len) {

        map<string, string> m;
        m[FILE_STRING] = get_file();
        m[SOCKET_STRING] = Utils::itoa(fd);

        if (addr != NULL && addr_len != NULL) {
            assert(sizeof (struct sockaddr_in) == *addr_len);
            AddressPort ap((struct sockaddr_in*) addr);
            m[ADDRESS_STRING] = ap.get_address();
            m[PORT_STRING] = Utils::itoa(ap.get_port());
            m[LENGTH_STRING] = Utils::itoa(*addr_len);
        }

        string message = QueryStringParser::create(WIFU_ACCEPT_NAME, m);
        send_to(write_file_, message);

        SocketData* data = sockets.get(fd);
        data->get_semaphore()->wait();


        socklen_t length = data->get_address_port_length();
        memcpy(addr_len, &length, sizeof (socklen_t));
        memcpy(addr, data->get_address_port()->get_network_struct_ptr(), (size_t) length);

        int new_socket = data->get_return_value();
        sockets.put(new_socket, new SocketData());

        return new_socket;
    }

    /**
     * Possibly Blocking
     * Send data to a remote destination
     * See man 2 send
     *
     * Creates a wifu_send message and passes it to the back-end, then waits for a response on the specified fd.
     *
     * @param fd The socket id
     * @param buf The buffer to send
     * @param n The length of buf
     * @param flags See man 2 send
     *
     * @return On success, the number of characters sent.  If an error occurs, -1 is returned and ERRNO is set.
     */
    ssize_t wifu_send(int fd, const void* buf, size_t n, int flags) {
        return wifu_sendto(fd, buf, n, flags, 0, 0);
    }

    /**
     * Blocking
     * Receive data on a socket
     * See man 2 send
     *
     * Creates a wifu_receive message and passes it to the back-end, then waits for a response on the specified fd.
     *
     * @param fd The socket id to receive on
     * @param buf The buffer to fill upon recept of data
     * @param n The length of buf
     * @param flags See man 2 recv
     *
     * @return On success, the number of bytes received.  If an error occurs, -1 is returned and ERRNO is set.
     * The return value may also be 0 if the peer performed an orderly shutdown
     */
    ssize_t wifu_recv(int fd, void* buf, size_t n, int flags) {
        return wifu_recvfrom(fd, buf, n, flags, 0, 0);
    }

    /**
     * Possibly Blocking
     * Send data to a remote destination
     * See man 2 send
     *
     * Creates a wifu_send message and passes it to the back-end, then waits for a response on the specified fd.
     *
     * @param fd The socket id
     * @param buf The buffer to send
     * @param n The length of buf
     * @param flags See man 2 send
     * @param addr The address to send to
     * @param addr_len The length of addr
     *
     * @return On success, the number of characters sent.  If an error occurs, -1 is returned and ERRNO is set.
     */
    ssize_t wifu_sendto(int fd, const void* buf, size_t n, int flags, const struct sockaddr* addr, socklen_t addr_len) {
        map<string, string> m;
        m[FILE_STRING] = get_file();
        m[SOCKET_STRING] = Utils::itoa(fd);
        m[BUFFER_STRING] = string((const char*) buf, n);
        m[N_STRING] = Utils::itoa(n);
        cout << "wifu_sendto(), buffer: " << m[BUFFER_STRING] << endl;
        m[FLAGS_STRING] = Utils::itoa(flags);


        if (addr != 0 && addr_len != 0) {
            assert(sizeof (struct sockaddr_in) == addr_len);
            AddressPort ap((struct sockaddr_in*) addr);
            m[ADDRESS_STRING] = ap.get_address();
            m[PORT_STRING] = Utils::itoa(ap.get_port());
            m[LENGTH_STRING] = Utils::itoa(addr_len);
        }

        string message = QueryStringParser::create(WIFU_SENDTO_NAME, m);
        send_to(write_file_, message);

        SocketData* data = sockets.get(fd);
        data->get_semaphore()->wait();



        return data->get_return_value();
    }

    /**
     * Blocking
     * Receive data on a socket
     * See man 2 send
     *
     * Creates a wifu_receive message and passes it to the back-end, then waits for a response on the specified fd.
     *
     * @param fd The socket id to receive on
     * @param buf The buffer to fill upon recept of data
     * @param n The length of buf
     * @param flags See man 2 recv
     * @param addr The address to receive from
     * @param addr_len the length of addr
     *
     * @return On success, the number of bytes received.  If an error occurs, -1 is returned and ERRNO is set.
     * The return value may also be 0 if the peer performed an orderly shutdown
     */
    ssize_t wifu_recvfrom(int fd, void *__restrict buf, size_t n, int flags, struct sockaddr* addr, socklen_t *__restrict addr_len) {
        map<string, string> m;
        m[FILE_STRING] = get_file();
        m[SOCKET_STRING] = Utils::itoa(fd);
        m[N_STRING] = Utils::itoa(n);
        m[FLAGS_STRING] = Utils::itoa(flags);

        if (addr != 0 && addr_len != 0) {
            assert(sizeof (struct sockaddr_in) == *addr_len);
            AddressPort ap((struct sockaddr_in*) addr);
            m[ADDRESS_STRING] = ap.get_address();
            m[PORT_STRING] = Utils::itoa(ap.get_port());
            m[LENGTH_STRING] = Utils::itoa(*addr_len);
        }

        string message = QueryStringParser::create(WIFU_RECVFROM_NAME, m);
        send_to(write_file_, message);

        SocketData* data = sockets.get(fd);
        assert(data != NULL);
        assert(data->get_semaphore() != NULL);

        data->get_semaphore()->wait();
        ssize_t ret_val = data->get_return_value();

        // TODO: fill in the actual vale of addr_len according to man 2 recvfrom()

        if (ret_val > 0) {
            memcpy(buf, data->get_payload(), ret_val);
//            cout << "wifu_recvfrom(), buffer: " << (const char*) buf << endl;
        }
        return ret_val;
    }

    /**
     * Non-blocking
     * Connect to a remote peer
     * See man 2 connect
     *
     * Creates a wifu_connect message and passes it to the back-end, then waits for a response on the specified fd.
     *
     * @param fd The socket id
     * @param addr The address/port of the remote peer to connect to
     * @param len the length of addr
     *
     * @return 0 if call was successfull, -1 otherwise (and ERRNO is set appropriately)
     */
    int wifu_connect(int fd, const struct sockaddr* addr, socklen_t len) {
        assert(addr != NULL);
        assert(sizeof (struct sockaddr_in) == len);

        map<string, string> m;
        m[FILE_STRING] = get_file();
        m[SOCKET_STRING] = Utils::itoa(fd);

        AddressPort ap((struct sockaddr_in*) addr);
        m[ADDRESS_STRING] = ap.get_address();
        m[PORT_STRING] = Utils::itoa(ap.get_port());
        m[LENGTH_STRING] = Utils::itoa(len);

        string message = QueryStringParser::create(WIFU_CONNECT_NAME, m);
        send_to(write_file_, message);

        SocketData* data = sockets.get(fd);
        data->get_semaphore()->wait();
        return data->get_return_value();

    }

private:
    /**
     * The file this WifuEndAPILocalSocket will write to in order to send messages to the back-end (usually /tmp/WS)
     */
    string write_file_;

    /**
     * Special Semaphore used to indicate we are sending/receiving a wifu_socket message.
     */
    BinarySemaphore socket_signal_;

    /**
     * Semaphore to only allow one call to wifu_socket at a time.
     */
    BinarySemaphore socket_mutex_;

    /**
     * Response received from the back-end.  The key is the method name, the value is the message.
     */
    map<string, string> response_;


};


#endif	/* _WIFUENDAPILOCALSOCKET_H */


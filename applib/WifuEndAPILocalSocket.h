/* 
 * File:   WifuEndAPILocalSocket.h
 * Author: rbuck
 *
 * Created on November 22, 2010, 3:10 PM
 */

#ifndef _WIFUENDAPILOCALSOCKET_H
#define	_WIFUENDAPILOCALSOCKET_H

#include "SocketData.h"


#include <sstream>

#include "../headers/QueryStringParser.h"
#include "../headers/LocalSocketFullDuplex.h"
#include "../headers/LocalSocketReceiverCallback.h"
#include "../headers/BinarySemaphore.h"
#include "../headers/Utils.h"
#include "../headers/AddressPort.h"
#include "../headers/defines.h"
#include "SocketMap.h"

#define sockets SocketMap::instance()

// TODO: Go over each man page and determine what we want to support,
// TODO: then make sure that every function in this file supports that behavior.

class WifuEndAPILocalSocket : public LocalSocketFullDuplex {
private:

    /**
     * Constructs a WifuEndAPILocalSocket for use in communicating with the Wifu End process
     *
     * @param file The file which this object listens on (other local sockets can write to this file).
     */
    WifuEndAPILocalSocket() : LocalSocketFullDuplex("LibrarySocket"), write_file_("WifuSocket") {
        socket_signal_.init(0);
        socket_mutex_.init(1);
    }

    WifuEndAPILocalSocket(WifuEndAPILocalSocket const&) : LocalSocketFullDuplex("LibrarySocket"), write_file_("WifuSocket") {

    }

    WifuEndAPILocalSocket & operator=(WifuEndAPILocalSocket const&) {

    }

public:

    static WifuEndAPILocalSocket& instance() {
        static WifuEndAPILocalSocket instance_;
        return instance_;
    }

    virtual ~WifuEndAPILocalSocket() {

    }

    /**
     * This is the callback function where messages received come
     *
     * @param message The message received
     */
    void receive(string& message) {
        //cout << "Response:\t" << message << endl;
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
            sockets.get(socket)->set_payload(response_[BUFFER_STRING]);
        }

        int value = atoi(response_[RETURN_VALUE_STRING].c_str());
        SocketData* data = sockets.get(socket);
        data->set_return_value(value);
        data->get_semaphore()->post();
    }

    /**
     * Non-blocking
     *
     */
    int wifu_socket(int domain, int type, int protocol) {
        socket_mutex_.wait();
        map<string, string> m;
        m[FILE_STRING] = getFile();
        m[DOMAIN_STRING] = Utils::itoa(domain);
        m[TYPE_STRING] = Utils::itoa(type);
        m[PROTOCOL_STRING] = Utils::itoa(protocol);
        string message = QueryStringParser::create(WIFU_SOCKET_NAME, m);
        send_to(write_file_, message);

        socket_signal_.wait();

        // TODO: Ensure that we never receive a socket id of 0        
        SocketData* data = sockets.get(0);
        int socket = data->get_return_value();
        sockets.erase_at(0);
        sockets.put(socket, data);

        socket_mutex_.post();
        return socket;
    }

    /**
     * Non-blocking
     */
    int wifu_bind(int fd, const struct sockaddr* addr, socklen_t len) {
        assert(sizeof (struct sockaddr_in) == len);
        AddressPort ap((struct sockaddr_in*) addr);

        map<string, string> m;
        m[FILE_STRING] = getFile();
        m[SOCKET_STRING] = Utils::itoa(fd);
        m[ADDRESS_STRING] = ap.get_address();
        m[PORT_STRING] = Utils::itoa(ap.get_port());
        m[LENGTH_STRING] = Utils::itoa(len);

        string message = QueryStringParser::create(WIFU_BIND_NAME, m);
        send_to(write_file_, message);

        SocketData* data = sockets.get(fd);
        data->get_semaphore()->wait();
        return data->get_return_value();
    }

    /**
     * Non-blocking
     */
    int wifu_listen(int fd, int n) {
        map<string, string> m;
        m[FILE_STRING] = getFile();
        m[SOCKET_STRING] = Utils::itoa(fd);
        m[N_STRING] = Utils::itoa(n);

        string message = QueryStringParser::create(WIFU_LISTEN_NAME, m);
        send_to(write_file_, message);

        SocketData* data = sockets.get(fd);
        data->get_semaphore()->wait();
        return data->get_return_value();
    }

    /**
     * Blocking
     */
    int wifu_accept(int fd, struct sockaddr* addr, socklen_t *__restrict addr_len) {

        map<string, string> m;
        m[FILE_STRING] = getFile();
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

        // TODO: Fill in addr_len according to man 2 accept

        return data->get_return_value();
    }

    /**
     * Possibly Blocking (man send 2)
     */
    ssize_t wifu_send(int fd, const void* buf, size_t n, int flags) {
        return wifu_sendto(fd, buf, n, flags, 0, 0);
    }

    /**
     * Blocking
     */
    ssize_t wifu_recv(int fd, void* buf, size_t n, int flags) {
        return wifu_recvfrom(fd, buf, n, flags, 0, 0);
    }

    /**
     * Possibly Blocking (man send 2)
     */
    ssize_t wifu_sendto(int fd, const void* buf, size_t n, int flags, const struct sockaddr* addr, socklen_t addr_len) {
        map<string, string> m;
        m[FILE_STRING] = getFile();
        m[SOCKET_STRING] = Utils::itoa(fd);
        m[BUFFER_STRING] = string((const char*) buf, n);
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
     */
    ssize_t wifu_recvfrom(int fd, void *__restrict buf, size_t n, int flags, struct sockaddr* addr, socklen_t *__restrict addr_len) {
        map<string, string> m;
        m[FILE_STRING] = getFile();
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
        data->get_semaphore()->wait();
        ssize_t ret_val = data->get_return_value();

        // TODO: fill in the actual vale of addr_len according to man 2 recvfrom()

        if (ret_val > 0) {
            memcpy(buf, data->get_payload(), ret_val);
        }
        return ret_val;
    }

    /**
     * Non-blocking
     */
    int wifu_connect(int fd, const struct sockaddr* addr, socklen_t len) {
        assert(addr != NULL);
        assert(sizeof (struct sockaddr_in) == len);

        map<string, string> m;
        m[FILE_STRING] = getFile();
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
    string write_file_;
    BinarySemaphore socket_signal_;
    BinarySemaphore socket_mutex_;
    map<string, string> response_;


};


#endif	/* _WIFUENDAPILOCALSOCKET_H */


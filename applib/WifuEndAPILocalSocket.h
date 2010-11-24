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

class WifuEndAPILocalSocket : public LocalSocketFullDuplex {
public:

    /**
     * Constructs a WifuEndAPILocalSocket for use in communicating with the Wifu End process
     *
     * @param file The file which this object listens on (other local sockets can write to this file).
     */
    WifuEndAPILocalSocket(string& file) : LocalSocketFullDuplex(file) {
        write_file_ = "WifuSocket";
        socket_sem_.init(0);
        socket_mutex_.init(1);
    }

    virtual ~WifuEndAPILocalSocket() {

    }

    /**
     * This is the callback function where messages received come
     *
     * @param message The message received
     */
    void receive(string& message) {
        response_.clear();
        QueryStringParser::parse(message, response_);
        int socket = atoi(response_[SOCKET_STRING].c_str());

        if(!response_[NAME_STRING].compare("wifu_socket")) {
            sockets.put(0, new SocketData());
            sockets.get(0)->set_return_value(socket);
            socket_sem_.post();
            return;
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
        m["domain"] = Utils::itoa(domain);
        m["type"] = Utils::itoa(type);
        m["protocol"] = Utils::itoa(protocol);
        string message = QueryStringParser::create("wifu_socket", m);
        send_to(write_file_, message);
        
        socket_sem_.wait();

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
        assert(sizeof(struct sockaddr_in) == len);
        AddressPort ap((struct sockaddr_in*)addr);

        map<string, string> m;
        m[FILE_STRING] = getFile();
        m[SOCKET_STRING] = Utils::itoa(fd);
        m["address"] = ap.get_address();
        m["port"] = Utils::itoa(ap.get_port());
        m["length"] = Utils::itoa(len);

        string message = QueryStringParser::create("wifu_bind", m);
        send_to(write_file_, message);

        SocketData* data = sockets.get(fd);
        data->get_semaphore()->wait();
        return data->get_return_value();
    }

    /**
     * Non-blocking
     */
    int wifu_listen(int fd, int n) {
        return 0;
    }

    /**
     * Blocking
     */
    int wifu_accept(int fd, struct sockaddr* addr, socklen_t *__restrict addr_len) {
        return 0;
    }

    /**
     * Possibly Blocking (man send 2)
     */
    ssize_t wifu_send(int fd, const void* buf, size_t n, int flags) {
        return 0;
    }

    /**
     * Blocking
     */
    ssize_t wifu_recv(int fd, void* buf, size_t n, int flags) {
        return 0;
    }

    /**
     * Possibly Blocking (man send 2)
     */
    ssize_t wifu_sendto(int fd, const void* buf, size_t n, int flags, const struct sockaddr* addr, socklen_t addr_len) {
        return 0;
    }

    /**
     * Blocking
     */
    ssize_t wifu_recvfrom(int fd, void *__restrict buf, size_t n, int flags, struct sockaddr* addr, socklen_t *__restrict addr_len) {
        return 0;
    }

    /**
     * Non-blocking
     */
    int wifu_connect(int fd, const struct sockaddr* addr, socklen_t len) {
        return 0;
    }

private:
    string write_file_;
    BinarySemaphore socket_sem_;
    BinarySemaphore socket_mutex_;
    map<string, string> response_;

};


#endif	/* _WIFUENDAPILOCALSOCKET_H */


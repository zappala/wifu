/* 
 * File:   WifuEndAPILocalSocket.h
 * Author: rbuck
 *
 * Created on November 22, 2010, 3:10 PM
 */

#ifndef _WIFUENDAPILOCALSOCKET_H
#define	_WIFUENDAPILOCALSOCKET_H

#include <sstream>

#include "../headers/LocalSocketFullDuplex.h"
#include "../headers/LocalSocketReceiverCallback.h"
#include "../headers/Semaphore.h"

class WifuEndAPILocalSocket : public LocalSocketFullDuplex {
public:

    /**
     * Constructs a WifuEndAPILocalSocket for use in communicating with the Wifu End process
     *
     * @param file The file which this object listens on (other local sockets can write to this file).
     */
    WifuEndAPILocalSocket(string& file) : LocalSocketFullDuplex(file) {
        write_file_ = "WifuSocket";
        sem_.init(0);
    }

    virtual ~WifuEndAPILocalSocket() {

    }

    /**
     * This is the callback function where messages received come
     *
     * @param message The message received
     */
    void receive(string& message) {
        cout << message << endl;
        sem_.post();
    }

    int wifu_socket(int domain, int type, int protocol) {
        stringstream s;
        s << "wifu_socket?file=";
        s << getFile() << "&";
        s << "domain=";
        s << domain << "&";
        s << "type=";
        s << type << "&";
        s << "protocol=";
        s << protocol;
        string data = s.str();
        send_to(write_file_, data);

        sem_.wait();

        return 0;
        
    }

    int wifu_bind(int fd, const struct sockaddr* addr, socklen_t len) {
        return 0;
    }

    int wifu_listen(int fd, int n) {
        return 0;
    }

    int wifu_accept(int fd, struct sockaddr* addr, socklen_t *__restrict addr_len) {
        return 0;
    }

    ssize_t wifu_send(int fd, const void* buf, size_t n, int flags) {
        return 0;
    }

    ssize_t wifu_recv(int fd, void* buf, size_t n, int flags) {
        return 0;
    }

    ssize_t wifu_sendto(int fd, const void* buf, size_t n, int flags, const struct sockaddr* addr, socklen_t addr_len) {
        return 0;
    }

    ssize_t wifu_recvfrom(int fd, void *__restrict buf, size_t n, int flags, struct sockaddr* addr, socklen_t *__restrict addr_len) {
        return 0;
    }

    int wifu_connect(int fd, const struct sockaddr* addr, socklen_t len) {
        return 0;
    }

private:
    string write_file_;
    Semaphore sem_;

};


#endif	/* _WIFUENDAPILOCALSOCKET_H */


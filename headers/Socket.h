/* 
 * File:   Socket.h
 * Author: rbuck
 *
 * Created on October 30, 2010, 9:18 AM
 */

#ifndef _SOCKET_H
#define	_SOCKET_H

#include <string>

#include "QueueProcessor.h"
using namespace std;

class Socket : public Identifiable {
public:
    Socket() {

    }

    virtual ~Socket() {

    }

    void connect(string & address) {
        Event* connect_event = new ConnectEvent(get_socket(), address);
        dispatch(connect_event);
    }

    void send(string & message) {

    }

    int & get_socket() {
        return get_id();
    }

private:
    void dispatch(Event * e) {
        Dispatcher::instance().enqueue(e);
    }
};

#endif	/* _SOCKET_H */


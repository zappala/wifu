/* 
 * File:   Socket.h
 * Author: rbuck
 *
 * Created on October 20, 2010, 11:46 AM
 */

#ifndef _SOCKET_H
#define	_SOCKET_H

#include "LocalSocketSender.h"
#include "Dispatcher.h"


#include <string>

using namespace std;

class Socket : public LocalSocketFullDuplex {
public:

    Socket(string & file) : LocalSocketFullDuplex(file) {
        
    }

    void connect(string & address) {
        string dispatcher = Dispatcher::instance().getFile();
        string message("<connect> ");
        message.append(address);
        send_to(dispatcher, message);
    }

    void send(string & message) {
        
    }

    void receive(string &  message) {
        cout << "Message Received" << endl;
    }

};

#endif	/* _SOCKET_H */


/* 
 * File:   SocketCongestionControl.h
 * Author: rbuck
 *
 * Created on October 20, 2010, 11:47 AM
 */

#ifndef _SOCKETCONGESTIONCONTROL_H
#define	_SOCKETCONGESTIONCONTROL_H

#include "LocalSocketFullDuplex.h"
#include "ISocketModule.h"

/*
 * @depricated This class uses Unix sockets; we are now using Events instead.
 */
class SocketCongestionControl : public LocalSocketFullDuplex, public ISocketModule {
public:

    SocketCongestionControl(string & file) : LocalSocketFullDuplex(file) {

    }

    ~SocketCongestionControl() {

    }

    void receive(string & message) {
        cout << "CongestionControl Receive: " << message << endl;
    }

    void connect(string & message) {
        cout << "CongestionControl connect: " << message << endl;
    }

    void data(string & message) {
        cout << "CongestionControl data: " << message << endl;
    }
};

#endif	/* _SOCKETCONGESTIONCONTROL_H */


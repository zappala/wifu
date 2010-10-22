/* 
 * File:   CongestionControl.h
 * Author: rbuck
 *
 * Created on October 20, 2010, 11:47 AM
 */

#ifndef _CONGESTIONCONTROL_H
#define	_CONGESTIONCONTROL_H

#include "LocalSocketFullDuplex.h"
#include "IModule.h"

class CongestionControl : public LocalSocketFullDuplex, public IModule {
public:

    CongestionControl(string & file) : LocalSocketFullDuplex(file) {

    }

    ~CongestionControl() {

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

#endif	/* _CONGESTIONCONTROL_H */


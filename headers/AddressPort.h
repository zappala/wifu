/* 
 * File:   AddressPort.h
 * Author: rbuck
 *
 * Created on October 30, 2010, 10:06 AM
 */

#ifndef _ADDRESSPORT_H
#define	_ADDRESSPORT_H

#include <string>

using namespace std;

class AddressPort {
public:
    AddressPort(string & address, string & port) : address_(address), port_(port) {

    }

    virtual ~AddressPort() {

    }

    string & get_address() {
        return address_;
    }

    string & get_port() {
        return port_;
    }

private:
    string address_;
    string port_;
};

#endif	/* _ADDRESSPORT_H */


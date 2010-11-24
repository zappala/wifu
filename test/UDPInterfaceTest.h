/* 
 * File:   UDPInterfaceTest.h
 * Author: rbuck
 *
 * Created on November 19, 2010, 3:09 PM
 */

#ifndef _UDPINTERFACETEST_H
#define	_UDPINTERFACETEST_H

#include <iostream>
#include <string>
#include <vector>

#include "headers/UnitTest++.h"
#include "../headers/UDPInterface.h"

using namespace std;

class UDPSocketCallbackImpl : public UDPSocketCallback {
public:

    UDPSocketCallbackImpl() : UDPSocketCallback(), ap_(0) {

    }

    virtual ~UDPSocketCallbackImpl() {
        if (ap_) {
            delete ap_;
        }
    }

    void receive(AddressPort& ap, unsigned char* buffer, size_t length) {
        if (ap_) {
            delete ap_;
        }

        ap_ = new AddressPort(ap.get_network_struct_ptr());
        message_ = string((char*) buffer);
    }

    AddressPort* get_ap() {
        return ap_;
    }

    string& get_message() {
        return message_;
    }

private:
    AddressPort* ap_;
    string message_;
};


namespace {

    SUITE(UDPInterface) {

        TEST(UDPInterfaceTest) {
            
        }
    }
}

#endif	/* _UDPINTERFACETEST_H */


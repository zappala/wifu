/* 
 * File:   UDPSocketTest.h
 * Author: rbuck
 *
 * Created on November 15, 2010, 1:49 PM
 */

#ifndef _UDPSOCKETTEST_H
#define	_UDPSOCKETTEST_H
#include <iostream>
#include <string>
#include <vector>

#include "UnitTest++.h"
#include "../headers/AddressPort.h"
#include "../headers/UDPSocket.h"

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

    SUITE(UDPSocket) {

        TEST(UDPSocket) {

            UDPSocket sender;
            UDPSocket receiver;
            UDPSocketCallbackImpl callback;

            string address("127.0.0.1");
            int port = 5000;
            string message("message");
            AddressPort ap(address, port);

            receiver.bind_socket(ap);
            receiver.receive(&callback);

            usleep(500);

            sender.makeNonBlocking();
            size_t count = sender.send(ap, message);

            usleep(500);

            sender.closeSocket();
            receiver.closeSocket();

            CHECK_EQUAL(message.length(), count);
            CHECK_EQUAL(message, callback.get_message());
            CHECK_EQUAL(ap.get_address(), callback.get_ap()->get_address());
        }
    }
}


#endif	/* _UDPSOCKETTEST_H */



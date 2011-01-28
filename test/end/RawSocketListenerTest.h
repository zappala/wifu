/* 
 * File:   RawSocketListenerTest.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 2:12 PM
 */

#ifndef _RAWSOCKETLISTENERTEST_H
#define	_RAWSOCKETLISTENERTEST_H

#include "UnitTest++.h"
#include "../headers/AddressPort.h"
#include "../headers/NetworkCallback.h"
#include "../headers/RawSocketListener.h"
#include "../headers/RawSocketSender.h"

using namespace std;

class NetworkCallbackImpl : public NetworkCallback {
public:

    NetworkCallbackImpl() : NetworkCallback(), ap_(0) {

    }

    virtual ~NetworkCallbackImpl() {
        if (ap_) {
            delete ap_;
        }
    }

    void receive(WiFuPacket* p) {
        message_ = (const char*) p->get_data();
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

    SUITE(RawSocketListener) {

        TEST(RawSocketListener) {

            RawSocketSender sender;
            RawSocketListener listener;
            NetworkCallbackImpl callback;

//            string address("127.0.0.1");
//            int port = 5000;
//            string message("message");
//            AddressPort ap(address, port);
//
//            receiver.bind_socket(ap);
//            receiver.receive(&callback);
//
//            usleep(5000);
//
//            sender.makeNonBlocking();
//            size_t count = sender.send(ap, message);
//
//            usleep(5000);
//
//            //            if (sender.closeSocket() < 0) {
//            //                cout << "Error closing sender (errno): " << errno << endl;
//            //            }
//
//            //            if (receiver.closeSocket() < 0) {
//            //                cout << "Error closing receiver (errno): " << errno << endl;
//            //            }
//
//            CHECK_EQUAL(message.length(), count);
//            CHECK_EQUAL(message, callback.get_message());
//            CHECK_EQUAL(ap.get_address(), callback.get_ap()->get_address());

        }
    }
}

#endif	/* _RAWSOCKETLISTENERTEST_H */


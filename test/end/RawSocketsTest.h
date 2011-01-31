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
#include "../headers/PacketFactory.h"
#include "../headers/packet/WiFuPacket.h"
#include "../headers/WiFuPacketFactory.h"
#include "RandomStringGenerator.h"

#define random_string(x) RandomStringGenerator::get_data(x)

using namespace std;

class NetworkCallbackImpl : public NetworkCallback {
public:

    NetworkCallbackImpl() : NetworkCallback(), ap_(0) {
        sem_.init(0);
    }

    virtual ~NetworkCallbackImpl() {
        if (ap_) {
            delete ap_;
        }
    }

    void receive(WiFuPacket* p) {
        message_ = (const char*) p->get_data();
        sem_.post();
    }

    AddressPort* get_ap() {
        return ap_;
    }

    string& get_message() {
        return message_;
    }

    Semaphore& get_sem() {
        return sem_;
    }

private:
    AddressPort* ap_;
    string message_;
    Semaphore sem_;
};



WiFuPacket* make_packet(string& data) {
    WiFuPacket* p = new WiFuPacket();

    p->set_ip_length(5);
    p->set_ip_version(4);
    p->set_ip_tos(0);
    p->set_ip_destination_address_s("127.0.0.1");
    p->set_destination_port(5000);
    p->set_ip_protocol(100);
    p->set_source_port(5001);
    p->set_ip_ttl(65);
    p->set_data((unsigned char*) data.c_str(), data.length());
    return p;


}


namespace {

    SUITE(RawSockets) {

        TEST(RawSockets) {

            RawSocketSender sender;
            RawSocketListener listener;
            NetworkCallbackImpl callback;

            listener.register_protocol(100, new WiFuPacketFactory());
            listener.start(&callback);

            string data = random_string(1000);

            sender.send(make_packet(data));

            callback.get_sem().wait();

            CHECK_EQUAL(data, callback.get_message());



        }
    }
}

#endif	/* _RAWSOCKETLISTENERTEST_H */


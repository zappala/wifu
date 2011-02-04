/* 
 * File:   RawSocketListenerTest.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 2:12 PM
 */

#include "gtest/gtest.h"
#include "AddressPort.h"
#include "NetworkCallback.h"
#include "RawSocketListener.h"
#include "RawSocketSender.h"
#include "PacketFactory.h"
#include "packet/WiFuPacket.h"
#include "WiFuPacketFactory.h"
#include "headers/RandomStringGenerator.h"

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

    TEST(RawSockets, all) {

        RawSocketSender sender;
        RawSocketListener listener;
        NetworkCallbackImpl callback;

        listener.register_protocol(100, new WiFuPacketFactory());
        listener.start(&callback);

        for (int i = 0; i < 100; i++) {
            string data = random_string(1000);
            sender.send(make_packet(data));
            callback.get_sem().wait();
            ASSERT_EQ(data, callback.get_message());
        }
    }
}

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

void get_packet(AddressPort& destination, unsigned char* datagram) {
    struct iphdr *iph = (struct iphdr *) datagram;
    struct tcphdr* tcp = (struct tcphdr*) (datagram + sizeof (struct ip));
    struct sockaddr_in sin;
    /* the sockaddr_in containing the dest. address is used
       in sendto() to determine the datagrams path */

    memset(datagram, 0, 1500); /* zero out the buffer */

    const char* data = "This is some data";
    char* payload = (char*) (datagram + sizeof (struct ip) + sizeof (struct tcphdr));
    memcpy(payload, data, strlen(data));


    /* we'll now fill in the ip/tcp header values, see above for explanations */
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof (struct ip) + sizeof (struct tcphdr) + strlen(data); // Always filled in by kernel, but I'm not sure
    iph->id = 0; // Filled in by kernel if 0
    iph->frag_off = 0;
    iph->ttl = 65;
    iph->protocol = destination.get_port();
    iph->check = 0; /* Always filled in by kernel */
    iph->saddr = 0; //inet_addr(source_address.c_str());
    iph->daddr = inet_addr(destination.get_address().c_str());

    tcp->source = destination.get_port();
    tcp->dest = destination.get_port();
}

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

    int length = sizeof(struct iphdr) + sizeof(struct wifu_packet_header) + data.length();
    p->set_ip_datagram_length(length);

    strncpy((char*) p->get_data(), data.c_str(), data.length());
    return p;
    

}


namespace {

    SUITE(RawSocketListener) {

        TEST(RawSocketListener) {

            RawSocketSender sender;
            RawSocketListener listener;
            NetworkCallbackImpl callback;

            listener.register_protocol(100, new WiFuPacketFactory());
            listener.start(&callback);

            string data = "This is some data";

            sender.send(make_packet(data));

            callback.get_sem().wait();

            CHECK_EQUAL(data, callback.get_message());



        }
    }
}

#endif	/* _RAWSOCKETLISTENERTEST_H */


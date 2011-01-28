/* 
 * File:   NetworkInterface.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 11:50 AM
 */

#ifndef _NETWORKINTERFACE_H
#define	_NETWORKINTERFACE_H

#include "events/NetworkSendPacketEvent.h"
#include "NetworkCallback.h"
#include "Module.h"

class NetworkInterface : public Module, public NetworkCallback {
public:

    static NetworkInterface& instance() {
        static NetworkInterface instance_;
        return instance_;
    }

    void start() {

    }

    virtual ~NetworkInterface() {

    }

    void receive(WiFuPacket* p) {

    }

    void network_send(Event* e) {
        NetworkSendPacketEvent* event = (NetworkSendPacketEvent*) e;
        WiFuPacket* p = event->get_packet();

        if(sendto( socket_,
                p->get_payload(),
                p->get_ip_length_bytes(),
                0,
                (struct sockaddr*) p->get_dest_address_port(),
                (sizeof(struct sockaddr_in))) < 0) {
            cout << "NetworkInterface: Error Sending Packet" << endl;
        }

    }

private:

    NetworkInterface() : Module(), NetworkCallback() {
        create_socket();
        set_header_include();
    }

    void create_socket() {
        socket_ = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
        if (socket_ < 0) {
            perror("NetworkInterface cannot create raw socket");
            exit(EXIT_FAILURE);
        }
    }

    void set_header_include() {
        int one = 1;
        const int *val = &one;
        if (setsockopt(socket_, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0) {
            perror("NetworkInterface: cannot set HDRINCL");
            exit(EXIT_FAILURE);
        }

    }

    int socket_;
};

#endif	/* _NETWORKINTERFACE_H */


/* 
 * File:   NetworkInterface.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 11:50 AM
 */

#ifndef _NETWORKINTERFACE_H
#define	_NETWORKINTERFACE_H

#include "events/NetworkSendPacketEvent.h"
#include "RawSocketSender.h"
#include "NetworkCallback.h"
#include "Module.h"
#include "RawSocketListener.h"

class NetworkInterface : public Module, public NetworkCallback {
public:

    static NetworkInterface& instance() {
        static NetworkInterface instance_;
        return instance_;
    }

    virtual ~NetworkInterface() {

    }

    void start() {
        listener_.start(this);
    }

    void register_protocol(int protocol, PacketFactory* pf){
        listener_.register_protocol(protocol, pf);
    }

    void receive(WiFuPacket* p) {

    }

    void network_send(Event* e) {
        NetworkSendPacketEvent* event = (NetworkSendPacketEvent*) e;
        WiFuPacket* p = event->get_packet();
        // TODO: Check return value (bytes sent)?
        sender_.send(p);
    }

private:

    NetworkInterface() : Module(), NetworkCallback() {

    }    

    int socket_;
    map<int, PacketFactory*> protocols_;
    RawSocketSender sender_;
    RawSocketListener listener_;
};

#endif	/* _NETWORKINTERFACE_H */


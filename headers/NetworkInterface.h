/* 
 * File:   NetworkInterface.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 11:50 AM
 */

#ifndef _NETWORKINTERFACE_H
#define	_NETWORKINTERFACE_H

#include "events/NetworkSendPacketEvent.h"
#include "events/NetworkReceivePacketEvent.h"
#include "RawSocketSender.h"
#include "NetworkCallback.h"
#include "Module.h"
#include "RawSocketListener.h"
#include "AddressPort.h"

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

    void register_protocol(int protocol, PacketFactory* pf) {
        listener_.register_protocol(protocol, pf);
    }

    void receive(WiFuPacket* p) {
        AddressPort* remote = p->get_source_address_port();
        AddressPort* local = p->get_dest_address_port();

        Socket* s = SocketCollection::instance().get_by_local_and_remote_ap(local, remote);

        if (!s) {
            s = SocketCollection::instance().get_by_local_ap(local);
        }

        if (!s) {
            // No bound local socket
            return;
        }

        Event* e = new NetworkReceivePacketEvent(s, p);
        Dispatcher::instance().enqueue(e);
    }

    void network_send(Event* e) {
        NetworkSendPacketEvent* event = (NetworkSendPacketEvent*) e;
        // TODO: Check return value (bytes sent)?
        sender_.send(event->get_packet());
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


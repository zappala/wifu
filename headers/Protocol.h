/* 
 * File:   Protocol.h
 * Author: rbuck
 *
 * Created on December 27, 2010, 2:17 PM
 */

#ifndef PROTOCOL_H
#define	PROTOCOL_H

#include "Module.h"
#include "events/LibraryEvent.h"
#include "events/ResponseEvent.h"
#include "events/SocketEvent.h"
#include "events/BindEvent.h"
#include "events/ListenEvent.h"
#include "HashSet.h"
#include "Socket.h"
#include "visitors/AlreadyListeningOnSamePortVisitor.h"
#include "visitors/AlreadyBoundToAddressPortVisitor.h"
#include "SocketCollection.h"

#include "contexts/ProtocolContext.h"
#include "contexts/ContextContainer.h"
#include "events/SendPacketEvent.h"
#include "events/UDPReceivePacketEvent.h"

class Protocol : public Module {
public:

    Protocol(int protocol) : Module(), protocol_(protocol) {

    }

    virtual ~Protocol() {

    }

    virtual ContextContainer* get_contexts() = 0;

    /**
     * Default implementation of what a protocol will do when a socket Event is received
     * Associates the socket id with this Protocol
     * Dispatches a response
     * @param e SocketEvent
     */
    virtual void library_socket(Event* e) {
        SocketEvent* event = (SocketEvent*) e;

        Socket* socket = SocketCollection::instance().get_by_id(event->get_socket());
        // This is to filter out bind events which do not correspond to this Protocol
        // TODO: this will not (likely) scale well
        if (socket->get_protocol() != protocol_) {
            return;
        }

        int s = socket->get_socket();
        ProtocolContext* c = new ProtocolContext();
        c->set_contexts(get_contexts());

        sockets_[s] = c;

        ResponseEvent* response = new ResponseEvent(s, event->get_name(), event->get_map()[FILE_STRING]);
        response->put(ERRNO, Utils::itoa(0));
        response->put(RETURN_VALUE_STRING, Utils::itoa(0));

        dispatch(response);
    }

    /**
     * Default implementation of what a protocol will do when a bind Event is received
     * @param e BindEvent
     */
    virtual void library_bind(Event* e) {
        BindEvent* event = (BindEvent*) e;

        int error = 0;
        int return_val = -1;
        int s = event->get_socket();

        map<int, ProtocolContext*>::iterator itr = sockets_.find(s);
        if (itr == sockets_.end()) {
            return;
        }

        Socket* socket = SocketCollection::instance().get_by_id(s);

        // TODO: may not need this check, it is done on the front end.
        // TODO: I'm leaving it for now, just to be safe.
        if (socket != NULL) {

            u_int16_t port = atoi(event->get_map()[PORT_STRING].c_str());
            AddressPort* local = new AddressPort(event->get_map()[ADDRESS_STRING], port);

            // TODO: Check possible errors
            AlreadyBoundToAddressPortVisitor v(local);
            SocketCollection::instance().accept(&v);

            if (!v.is_bound()) {
                socket->set_local_address_port(local);
                return_val = 0;
            } else {
                error = EINVAL;
            }

        } else {
            error = EBADF;
        }

        ResponseEvent* response = new ResponseEvent(s, event->get_name(), event->get_map()[FILE_STRING]);
        response->put(ERRNO, Utils::itoa(error));
        response->put(RETURN_VALUE_STRING, Utils::itoa(return_val));

        dispatch(response);
    }

    virtual void library_listen(Event* e) {
        ListenEvent* event = (ListenEvent*) e;

        int s = event->get_socket();
        map<int, ProtocolContext*>::iterator itr = sockets_.find(s);
        if (itr == sockets_.end()) {
            return;
        }

        // TODO: Do something with this back log
        int back_log = atoi(event->get_map()[N_STRING].c_str());

        int error = 0;
        int return_val = 0;

        Socket* socket = SocketCollection::instance().get_by_id(s);

        // TODO: do we need to check the address and port or just the port?
        AlreadyListeningOnSamePortVisitor v(socket->get_local_address_port()->get_port());
        SocketCollection::instance().accept(&v);

        // TODO: check EOPNOTSUPP:
        // The socket is not of a type that supports  the  listen()  operation.
        // see man 2 listen

        if (v.is_listening()) {
            error = EADDRINUSE;
            return_val = -1;
        } else {
            // Call all contexts
            itr->second->listen(socket, back_log);
        }

        ResponseEvent* response = new ResponseEvent(s, event->get_name(), event->get_map()[FILE_STRING]);
        response->put(ERRNO, Utils::itoa(error));
        response->put(RETURN_VALUE_STRING, Utils::itoa(return_val));

        // TODO: we may not need this if we are pushing things into the FSMs
        socket->make_passive();
        dispatch(response);
    }

    // TODO: refactor this to use the template pattern.
    // Have common code here then abstract method calls which will
    // call the actual protocol implementaion by which the event can be
    // processed.
    virtual void library_connect(Event* e) {
        cout << "Library Connect" << endl;
        ConnectEvent* event = (ConnectEvent*) e;

        int s = event->get_socket();
        map<int, ProtocolContext*>::iterator itr = sockets_.find(s);
        if (itr == sockets_.end()) {
            return;
        }

        Socket* socket = SocketCollection::instance().get_by_id(s);

        // TODO: Error check

        string address = event->get_map()[ADDRESS_STRING];
        int port = atoi(event->get_map()[PORT_STRING].c_str());
        AddressPort ap(address, port);
        itr->second->connect(socket, ap);
        assert(false);

    }

    virtual void send(Event* e) {
        cout << "Send" << endl;
        SendPacketEvent* event = (SendPacketEvent*) e;

        int s = event->get_socket();
        map<int, ProtocolContext*>::iterator itr = sockets_.find(s);
        if (itr == sockets_.end()) {
            return;
        }

        Socket* socket = SocketCollection::instance().get_by_id(s);
        // TODO: Error check

        itr->second->send(socket, event->get_packet());
    }
    virtual void udp_receive(Event* e) {
        UDPReceivePacketEvent* event = (UDPReceivePacketEvent*)e;
        cout << "Packet received in Protocol!" << endl;
        
    }




private:
    int protocol_;

    /**
     * Socket id to ProtocolContext pointer map
     */
    map<int, ProtocolContext*> sockets_;
};

#endif	/* PROTOCOL_H */


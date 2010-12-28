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
#include "HashSet.h"

class Protocol : public Module {
public:

    Protocol(int protocol) : Module(), protocol_(protocol) {

    }

    virtual ~Protocol() {

    }

    /**
     * Default implementation of what a protocol will do when a bind Event is received
     * @param e BindEvent
     */
    virtual void library_bind(Event* e) {
        BindEvent* event = (BindEvent*) e;

        int return_val = -1;
        int s = event->get_socket();

        Socket* socket = SocketCollection::instance().get_by_id(event->get_socket());

        // This is to filter out bind events which do not correspond to this Protocol
        // TODO: this will not (likely) scale well
        if(socket->get_protocol() != protocol_) {
            return;
        }

        int error = 0;

        if (socket != NULL) {

            u_int16_t port = atoi(event->get_map()[PORT_STRING].c_str());
            AddressPort* local = new AddressPort(event->get_map()[ADDRESS_STRING], port);

            // TODO: Check possible errors

            AlreadyBoundToAddressPortVisitor v(local);
            SocketCollection::instance().accept(&v);

            if (!v.is_bound()) {
                // TODO: what is the best way to handle the mapping of sockets to protocols?
                sockets_.insert(s);
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

    HashSet<int>& get_sockets() {
        return sockets_;
    }

private:
    int protocol_;

    /**
     * Sockets which are associated with this protocol
     */
    HashSet<int> sockets_;
};

#endif	/* PROTOCOL_H */


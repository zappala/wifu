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
#include "events/NetworkReceivePacketEvent.h"
#include "events/Event.h"
#include "events/SendPacketEvent.h"
#include "events/ConnectEvent.h"
#include "events/AcceptEvent.h"
#include "events/ConnectionEstablishedEvent.h"
#include "events/TimerFiredEvent.h"
#include "events/ResendPacketEvent.h"
#include "events/CloseEvent.h"
#include "events/GetSocketOptionEvent.h"
#include "events/SetSocketOptionEvent.h"
#include "events/NetworkSendPacketEvent.h"

#include "HashSet.h"
#include "Socket.h"
#include "visitors/AlreadyListeningOnSamePortVisitor.h"
#include "visitors/AlreadyBoundToAddressPortVisitor.h"
#include "SocketCollection.h"
#include "contexts/IContext.h"

#include "events/ConnectionInitiatedEvent.h"
#include "events/ReceiveEvent.h"


#include "packet/TCPPacket.h"

class Protocol : public Module, public IContext {
public:

    Protocol(int protocol);
    virtual ~Protocol();

    /**
     * Default implementation of what a protocol will do when a socket Event is received
     * Associates the socket id with this Protocol
     * Dispatches a response
     * @param e SocketEvent
     */
    virtual void imodule_library_socket(Event* e);

    /**
     * Default implementation of what a protocol will do when a bind Event is received
     * @param e BindEvent
     */
    virtual void imodule_library_bind(Event* e);

    virtual void imodule_library_listen(Event* e);

    // TODO: refactor this to use the template pattern.
    // Have common code here then abstract method calls which will
    // call the actual protocol implementaion by which the event can be
    // processed.

    virtual void imodule_library_connect(Event* e);

    virtual void imodule_library_accept(Event* e);

    virtual void imodule_library_receive(Event* e);

    virtual void imodule_library_send(Event* e);

    virtual void imodule_library_close(Event* e);

    virtual void imodule_send(Event* e);

    virtual void imodule_network_receive(Event* e);

    virtual void imodule_connection_established(Event* e);

    virtual void imodule_connection_initiated(Event* e);

    virtual void imodule_timer_fired(Event* e);

    virtual void imodule_resend(Event* e);

    virtual void imodule_send_buffer_not_empty(Event* e);

    virtual void imodule_send_buffer_not_full(Event* e);

    virtual void imodule_receive_buffer_not_empty(Event* e);

    virtual void imodule_receive_buffer_not_full(Event* e);

    virtual void imodule_delete_socket(Event* e);

    virtual void imodule_library_set_socket_option(Event* e);

    virtual void imodule_library_get_socket_option(Event* e);

    virtual void send_network_packet(Socket* s, WiFuPacket* p);

    virtual bool should_enqueue(Event* event);

private:

    int protocol_;

    HashSet<Socket*> sockets_;

    // TODO: this needs to go elsewhere
    // Only using it so we don't send data to the application that we have already sent
    TCPPacket* last_received_;

};

#endif	/* PROTOCOL_H */


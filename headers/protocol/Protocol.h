/* 
 * File:   Protocol.h
 * Author: rbuck
 *
 * Created on December 27, 2010, 2:17 PM
 */

#ifndef PROTOCOL_H
#define	PROTOCOL_H

#include "Module.h"
#include "events/Event.h"
#include "events/framework_events/AcceptEvent.h"
#include "events/framework_events/BindEvent.h"
#include "events/framework_events/CancelTimerEvent.h"
#include "events/framework_events/CloseEvent.h"
#include "events/framework_events/ConnectEvent.h"
#include "events/protocol_events/ConnectionEstablishedEvent.h"
#include "events/protocol_events/ConnectionInitiatedEvent.h"
#include "events/protocol_events/DeleteSocketEvent.h"
#include "events/framework_events/GetSocketOptionEvent.h"
#include "events/framework_events/LibraryEvent.h"
#include "events/framework_events/ListenEvent.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"
#include "events/framework_events/NetworkSendPacketEvent.h"
#include "events/protocol_events/ReceiveBufferNotEmptyEvent.h"
#include "events/protocol_events/ReceiveBufferNotFullEvent.h"
#include "events/protocol_events/ReceiveBufferNotFullEvent.h"
#include "events/framework_events/ReceiveEvent.h"
#include "events/protocol_events/ResendPacketEvent.h"
#include "events/framework_events/ResponseEvent.h"
#include "events/protocol_events/SendBufferNotEmptyEvent.h"
#include "events/protocol_events/SendBufferNotFullEvent.h"
#include "events/framework_events/SendEvent.h"
#include "events/protocol_events/SendPacketEvent.h"
#include "events/framework_events/SetSocketOptionEvent.h"
#include "events/framework_events/SocketEvent.h"
#include "events/framework_events/SocketOptionEvent.h"
#include "events/framework_events/TimeoutEvent.h"
#include "events/framework_events/TimerEvent.h"
#include "events/framework_events/TimerFiredEvent.h"

#include "HashSet.h"
#include "Socket.h"
#include "visitors/AlreadyListeningOnSamePortVisitor.h"
#include "visitors/AlreadyBoundToAddressPortVisitor.h"
#include "SocketCollection.h"
#include "contexts/IContext.h"

#include "events/protocol_events/ConnectionInitiatedEvent.h"
#include "events/framework_events/ReceiveEvent.h"

#include "PriorityEventComparator.h"
#include "PriorityQueue.h"

#include "packet/TCPPacket.h"
#include "packet/ATPPacket.h"

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
};

#endif	/* PROTOCOL_H */


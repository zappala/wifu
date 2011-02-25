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

#include "HashSet.h"
#include "Socket.h"
#include "visitors/AlreadyListeningOnSamePortVisitor.h"
#include "visitors/AlreadyBoundToAddressPortVisitor.h"
#include "SocketCollection.h"
#include "contexts/IContext.h"


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
    virtual void library_socket(Event* e);

    /**
     * Default implementation of what a protocol will do when a bind Event is received
     * @param e BindEvent
     */
    virtual void library_bind(Event* e);

    virtual void library_listen(Event* e);

    // TODO: refactor this to use the template pattern.
    // Have common code here then abstract method calls which will
    // call the actual protocol implementaion by which the event can be
    // processed.

    virtual void library_connect(Event* e);

    virtual void library_accept(Event* e);

    virtual void send(Event* e);

    virtual void network_receive(Event* e);

    virtual void connection_established(Event* e);

    virtual void timer_fired(Event* e);

    virtual void resend(Event* e);
    
private:
    int protocol_;

    HashSet<Socket*> sockets_;
    

};

#endif	/* PROTOCOL_H */


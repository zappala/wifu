/* 
 * File:   UDP.h
 * Author: rbuck
 *
 * Created on January 11, 2012, 3:31 PM
 */

#ifndef SIMPLEUDP_H
#define	SIMPLEUDP_H

#include "Protocol.h"
#include "packet/UDPPacket.h"
#include "SourceGetter.h"
#include "SimpleUDPContainer.h"
#include "events/framework_events/RecvFromResponseEvent.h"


class SimpleUDP : public Protocol {
private:
    map<Socket*, SimpleUDPContainer*, std::less<Socket*>, gc_allocator<std::pair<Socket*, SimpleUDPContainer*> > > map_;

    void send_receive_response(SimpleUDPContainer* c);

protected:
    SimpleUDP();


public:

    static SimpleUDP& instance();
    virtual ~SimpleUDP();

    // IContext methods

    virtual void icontext_socket(QueueProcessor<Event*>* q, SocketEvent* e);

    virtual void icontext_bind(QueueProcessor<Event*>* q, BindEvent* e);

    virtual void icontext_listen(QueueProcessor<Event*>* q, ListenEvent* e);

    virtual void icontext_receive_packet(QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);

    virtual void icontext_send_packet(QueueProcessor<Event*>* q, SendPacketEvent* e);

    virtual void icontext_connect(QueueProcessor<Event*>* q, ConnectEvent* e);

    virtual void icontext_accept(QueueProcessor<Event*>* q, AcceptEvent* e);

    virtual void icontext_new_connection_established(QueueProcessor<Event*>* q, ConnectionEstablishedEvent* e);

    virtual void icontext_new_connection_initiated(QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e);

    virtual void icontext_close(QueueProcessor<Event*>* q, CloseEvent* e);

    virtual void icontext_timer_fired_event(QueueProcessor<Event*>* q, TimerFiredEvent* e);

    virtual void icontext_resend_packet(QueueProcessor<Event*>* q, ResendPacketEvent* e);

    virtual void icontext_send(QueueProcessor<Event*>* q, SendEvent* e);

    virtual bool icontext_can_send(Socket* s);

    virtual bool icontext_can_receive(Socket* s);

    virtual void icontext_receive(QueueProcessor<Event*>* q, ReceiveEvent* e);

    virtual void icontext_receive_buffer_not_empty(QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent* e);

    virtual void icontext_receive_buffer_not_full(QueueProcessor<Event*>* q, ReceiveBufferNotFullEvent* e);

    virtual void icontext_send_buffer_not_empty(QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e);

    virtual void icontext_send_buffer_not_full(QueueProcessor<Event*>* q, SendBufferNotFullEvent* e);

    virtual void icontext_delete_socket(QueueProcessor<Event*>* q, DeleteSocketEvent* e);

    virtual void icontext_set_socket_option(QueueProcessor<Event*>* q, SetSocketOptionEvent* e);

    virtual void icontext_get_socket_option(QueueProcessor<Event*>* q, GetSocketOptionEvent* e);
};

#endif	/* SIMPLEUDP_H */


/* 
 * File:   SimpleTCP.h
 * Author: rbuck
 *
 * Created on December 27, 2010, 3:13 PM
 */

#ifndef SIMPLETCP_H
#define	SIMPLETCP_H

#include <map>

#include "Protocol.h"
#include "defines.h"

#include "contexts/IContext.h"
#include "contexts/SimpleTCPIContextContainer.h"
#include "contexts/SimpleTCPReliabilityContext.h"
#include "events/framework_events/ReceiveEvent.h"
#include "events/protocol_events/ReceiveBufferNotEmptyEvent.h"
#include "events/protocol_events/SendBufferNotEmptyEvent.h"
#include "events/protocol_events/SendBufferNotFullEvent.h"
#include "events/framework_events/RecvFromResponseEvent.h"

#include "CacheMap.h"
#include "SimpleTCPCache.h"
#include "packet/TCPTimestampOption.h"

#include "ObjectPool.h"


class SimpleTCP : public Protocol {
private:
    SimpleTCP();

    void save_socket(Socket* s, SimpleTCPIContextContainer* icc = new SimpleTCPIContextContainer());

    SimpleTCPIContextContainer* get_context(Socket* s);

    map<Socket*, SimpleTCPIContextContainer*, std::less<Socket*>, gc_allocator<std::pair<Socket*, SimpleTCPIContextContainer*> > > map_;
    map<Socket*, SimpleTCPIContextContainer*, std::less<Socket*>, gc_allocator<std::pair<Socket*, SimpleTCPIContextContainer*> > >::iterator itr_;

    bool is_room_in_send_buffer(SendEvent* e);
    void save_in_buffer_and_send_events(SendEvent* e);
    void create_and_dispatch_received_data(ReceiveEvent* e);

    

public:
    static SimpleTCP& instance();

    virtual ~SimpleTCP();

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

#endif	/* SIMPLETCP_H */


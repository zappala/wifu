/* 
 * File:   SimpleTCP.h
 * Author: rbuck
 *
 * Created on December 27, 2010, 3:13 PM
 */

#ifndef SIMPLETCP_H
#define	SIMPLETCP_H

#include "Protocol.h"
#include "defines.h"

#include "contexts/IContext.h"
#include "contexts/SimpleTCPIContextContainer.h"
#include "contexts/SimpleTCPReliabilityContext.h"
#include "events/framework_events/ReceiveEvent.h"
#include "events/protocol_events/ReceiveBufferNotEmptyEvent.h"
#include "events/protocol_events/SendBufferNotEmptyEvent.h"
#include "events/protocol_events/SendBufferNotFullEvent.h"

#include "CacheMap.h"
#include "SimpleTCPCache.h"
#include "packet/TCPTimestampOption.h"

class SimpleTCP : public Protocol {
private:
    SimpleTCP();

    void save_socket(Socket* s, SimpleTCPIContextContainer* icc = new SimpleTCPIContextContainer());

    SimpleTCPIContextContainer* get_context(Socket* s);

    tr1::unordered_map<Socket*, SimpleTCPIContextContainer*> map_;
    tr1::unordered_map<Socket*, SimpleTCPIContextContainer*>::iterator itr_;

    bool is_room_in_send_buffer(SendEvent* e);
    void save_in_buffer_and_send_events(SendEvent* e);
    void create_and_dispatch_received_data(ReceiveEvent* e);

    

public:
    static SimpleTCP& instance();

    virtual ~SimpleTCP();

    void icontext_socket(QueueProcessor<Event*>* q, SocketEvent* e);

    void icontext_bind(QueueProcessor<Event*>* q, BindEvent* e);

    void icontext_listen(QueueProcessor<Event*>* q, ListenEvent* e);

    void icontext_receive_packet(QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);

    void icontext_send_packet(QueueProcessor<Event*>* q, SendPacketEvent* e);

    void icontext_connect(QueueProcessor<Event*>* q, ConnectEvent* e);

    void icontext_accept(QueueProcessor<Event*>* q, AcceptEvent* e);

    void icontext_new_connection_established(QueueProcessor<Event*>* q, ConnectionEstablishedEvent* e);

    void icontext_new_connection_initiated(QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e);

    void icontext_close(QueueProcessor<Event*>* q, CloseEvent* e);

    void icontext_timer_fired_event(QueueProcessor<Event*>* q, TimerFiredEvent* e);

    void icontext_resend_packet(QueueProcessor<Event*>* q, ResendPacketEvent* e);

    void icontext_send(QueueProcessor<Event*>* q, SendEvent* e);

    bool icontext_can_send(Socket* s);

    bool icontext_can_receive(Socket* s);

    void icontext_receive(QueueProcessor<Event*>* q, ReceiveEvent* e);

    void icontext_receive_buffer_not_empty(QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent* e);

    void icontext_receive_buffer_not_full(QueueProcessor<Event*>* q, ReceiveBufferNotFullEvent* e);

    void icontext_send_buffer_not_empty(QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e);

    void icontext_send_buffer_not_full(QueueProcessor<Event*>* q, SendBufferNotFullEvent* e);

    void icontext_delete_socket(QueueProcessor<Event*>* q, DeleteSocketEvent* e);

    void icontext_set_socket_option(QueueProcessor<Event*>* q, SetSocketOptionEvent* e);

    void icontext_get_socket_option(QueueProcessor<Event*>* q, GetSocketOptionEvent* e);
};

#endif	/* SIMPLETCP_H */


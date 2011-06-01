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

    void icontext_socket(SocketEvent* e);

    void icontext_bind(BindEvent* e);

    void icontext_listen(ListenEvent* e);

    void icontext_receive_packet(NetworkReceivePacketEvent* e);

    void icontext_send_packet(SendPacketEvent* e);

    void icontext_connect(ConnectEvent* e);

    void icontext_accept(AcceptEvent* e);

    void icontext_new_connection_established(ConnectionEstablishedEvent* e);

    void icontext_new_connection_initiated(ConnectionInitiatedEvent* e);

    void icontext_close(CloseEvent* e);

    void icontext_timer_fired_event(TimerFiredEvent* e);

    void icontext_resend_packet(ResendPacketEvent* e);

    void icontext_send(SendEvent* e);

    bool icontext_can_send(Socket* s);

    bool icontext_can_receive(Socket* s);

    void icontext_receive(ReceiveEvent* e);

    void icontext_receive_buffer_not_empty(ReceiveBufferNotEmptyEvent* e);

    void icontext_receive_buffer_not_full(ReceiveBufferNotFullEvent* e);

    void icontext_send_buffer_not_empty(SendBufferNotEmptyEvent* e);

    void icontext_send_buffer_not_full(SendBufferNotFullEvent* e);

    void icontext_delete_socket(DeleteSocketEvent* e);

    void icontext_set_socket_option(SetSocketOptionEvent* e);

    void icontext_get_socket_option(GetSocketOptionEvent* e);
};

#endif	/* SIMPLETCP_H */


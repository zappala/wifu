/* 
 * File:   TCPTahoe.h
 * Author: rbuck
 *
 * Created on May 4, 2011, 2:44 PM
 */

#ifndef TCPTAHOE_H
#define	TCPTAHOE_H

#include "Protocol.h"
#include "contexts/IContext.h"
#include "contexts/TCPTahoeIContextContainer.h"
#include "Math.h"
#include "HashSet.h"
#include "IContextContainerFactory.h"
#include "TCPTahoeIContextContainerFactory.h"
#include "contexts/BasicIContextContainer.h"

class TCPTahoe : public Protocol {
private:
    IContextContainerFactory* factory_;

protected:
    TCPTahoe(int protocol = TCP_TAHOE, IContextContainerFactory* factory = new TCPTahoeIContextContainerFactory());

    tr1::unordered_map<Socket*, BasicIContextContainer*> map_;

    HashSet<string> states_we_can_send_ack_;

    int get_available_room_in_send_buffer(SendEvent* e);
    void save_in_buffer_and_send_events(QueueProcessor<Event*>* q, SendEvent* e, int available_room_in_send_buffer);
    void create_and_dispatch_received_data(QueueProcessor<Event*>* q, ReceiveEvent* e);

    bool is_valid_sequence_number(TCPTahoeReliabilityContext* rc, TCPPacket* p);
    bool is_valid_ack_number(TCPTahoeReliabilityContext* rc, TCPPacket* p);

public:
    static TCPTahoe& instance();
    virtual ~TCPTahoe();

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

#endif	/* TCPTAHOE_H */


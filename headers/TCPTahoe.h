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

class TCPTahoe : public Protocol {
private:
    TCPTahoe();

    tr1::unordered_map<Socket*, TCPTahoeIContextContainer*> map_;

public:
    static TCPTahoe& instance();
    virtual ~TCPTahoe();

    // IContext methods

    void icontext_socket(SocketEvent* e);

    void icontext_bind(BindEvent* e);

    void icontext_listen(ListenEvent* e);

    void icontext_receive_packet(NetworkReceivePacketEvent* e);

    void icontext_send_packet(SendPacketEvent* e);

    void icontext_connect(ConnectEvent* e);

    void icontext_accept(AcceptEvent* e);

    void icontext_new_connection_established(ConnectionEstablishedEvent* e);

    void icontext_new_conneciton_initiated(ConnectionInitiatedEvent* e);

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

#endif	/* TCPTAHOE_H */


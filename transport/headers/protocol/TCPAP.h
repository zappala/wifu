/* 
 * File:   TCPAP.h
 * Author: leer
 *
 * Created on June 28, 2011, 3:22 PM
 */

#ifndef _TCPAP_H
#define	_TCPAP_H

#include "protocol/TCPTahoe.h"
#include "TCPAPIContextContainerFactory.h"

class TCPAP : public TCPTahoe {
public:
    TCPAP(int protocol = TCP_AP, IContextContainerFactory* = new TCPAPIContextContainerFactory());;
    virtual ~TCPAP();

    static TCPAP& instance();
    
    virtual void icontext_receive_packet(QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);

    virtual void icontext_send_packet(QueueProcessor<Event*>* q, SendPacketEvent* e);

    virtual void icontext_timer_fired_event(QueueProcessor<Event*>* q, TimerFiredEvent* e);

    virtual void icontext_set_socket_option(QueueProcessor<Event*>* q, SetSocketOptionEvent* e);
};

#endif	/* _TCPAP_H */


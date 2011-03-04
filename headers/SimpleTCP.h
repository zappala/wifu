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
#include "contexts/IContextContainer.h"
#include "contexts/ReliabilityContext.h"

class SimpleTCP : public Protocol {
private:
    SimpleTCP();

    void save_socket(Socket* s, IContextContainer* icc = new IContextContainer());

    IContextContainer* get_context(Socket* s);

    void send_network_packet(Socket* s, WiFuPacket* p);

    tr1::unordered_map<Socket*, IContextContainer*> map_;
    tr1::unordered_map<Socket*, IContextContainer*>::iterator itr_;

public:
    static SimpleTCP& instance();

    virtual ~SimpleTCP();

    void socket(SocketEvent* e);

    void bind(BindEvent* e);

    void listen(ListenEvent* e);

    void receive_packet(NetworkReceivePacketEvent* e);

    void send_packet(SendPacketEvent* e);

    void connect(ConnectEvent* e);

    void accept(AcceptEvent* e);

    void new_connection_established(ConnectionEstablishedEvent* e);

    void new_conneciton_initiated(ConnectionInitiatedEvent* e);

    void close();

    void timer_fired_event(TimerFiredEvent* e);

    void resend_packet(ResendPacketEvent* e);
};

#endif	/* SIMPLETCP_H */


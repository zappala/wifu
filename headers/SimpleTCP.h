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

    void save_socket(Socket* s);

    IContextContainer* get_context(Socket* s);

    tr1::unordered_map<Socket*, IContextContainer*> map_;
    tr1::unordered_map<Socket*, IContextContainer*>::iterator itr_;

public:
    static SimpleTCP& instance();

    virtual ~SimpleTCP();

    void socket(Socket* s);

    void bind(Socket* s, AddressPort* ap);

    void listen(Socket* s, int back_log);

    void receive_packet(Socket* s, WiFuPacket* p);

    void send_packet(Socket* s, WiFuPacket* p);

    void connect(ConnectEvent* e);

    void accept(AcceptEvent* e);

    void new_connection_established(Socket* s);

    void close();

    void timer_fired_event(TimerFiredEvent* e);
};

#endif	/* SIMPLETCP_H */


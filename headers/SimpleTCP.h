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

    void save_socket(Socket*);

    IContextContainer* get_context(Socket*);

    tr1::unordered_map<Socket*, IContextContainer*> map_;
    tr1::unordered_map<Socket*, IContextContainer*>::iterator itr_;

public:
    static SimpleTCP& instance();

    virtual ~SimpleTCP();

    void socket(Socket*);

    void bind(Socket*, AddressPort*);

    void listen(Socket*, int);

    void receive_packet(Socket*, WiFuPacket*);

    void send_packet(Socket*, WiFuPacket*);

    void connect(ConnectEvent*);

    void accept(AcceptEvent*);

    void new_connection_established(Socket*);

    void close();

    void timer_fired_event(TimerFiredEvent*);
};

#endif	/* SIMPLETCP_H */


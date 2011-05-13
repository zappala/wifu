/* 
 * File:   TCPTahoeIContextContainer.h
 * Author: rbuck
 *
 * Created on May 4, 2011, 3:55 PM
 */

#ifndef TCPTAHOEICONTEXTCONTAINER_H
#define	TCPTAHOEICONTEXTCONTAINER_H

#include "contexts/TCPTahoeCongestionControlContext.h"
#include "contexts/TCPTahoeReliabilityContext.h"
#include "contexts/ConnectionManagerContext.h"

#include "events/SendEvent.h"

class TCPTahoeIContextContainer : public gc {
public:
    TCPTahoeIContextContainer();
    ~TCPTahoeIContextContainer();

    IContext* get_connection_manager();
    IContext* get_congestion_control();
    IContext* get_reliability();

    SendEvent* get_saved_send_event();
    void set_saved_send_event(SendEvent* e);

    CloseEvent* get_saved_close_event();
    void set_saved_close_event(CloseEvent* e);

private:
    IContext* connection_manager_;
    IContext* congestion_control_;
    IContext* reliability_;

    SendEvent* saved_send_event_;
    CloseEvent* saved_close_event_;
};

#endif	/* TCPTAHOEICONTEXTCONTAINER_H */


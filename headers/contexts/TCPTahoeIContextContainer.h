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
//You can delete this one...
#include "contexts/TCPDelayedACKReliabilityContext.h"

#include "events/framework_events/SendEvent.h"
#include "events/framework_events/CloseEvent.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"

class TCPTahoeIContextContainer : public gc {
public:
    TCPTahoeIContextContainer();
    ~TCPTahoeIContextContainer();

    virtual IContext* get_connection_manager();
    virtual IContext* get_congestion_control();
    virtual IContext* get_reliability();

    SendEvent* get_saved_send_event();
    void set_saved_send_event(SendEvent* e);

    CloseEvent* get_saved_close_event();
    void set_saved_close_event(CloseEvent* e);

    NetworkReceivePacketEvent* get_saved_fin();
    void set_saved_fin(NetworkReceivePacketEvent* e);

    

private:
    IContext* connection_manager_;
    IContext* congestion_control_;
    IContext* reliability_;

    SendEvent* saved_send_event_;
    CloseEvent* saved_close_event_;
    NetworkReceivePacketEvent* fin_;
};

#endif	/* TCPTAHOEICONTEXTCONTAINER_H */


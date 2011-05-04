/* 
 * File:   ContextContainer.h
 * Author: rbuck
 *
 * Created on December 31, 2010, 9:52 AM
 */

#ifndef CONTEXTCONTAINER_H
#define	CONTEXTCONTAINER_H

#include "IContext.h"
#include "GarbageCollector.h"
#include "SimpleTCPReliabilityContext.h"
#include "SimpleTCPCongestionControlContext.h"
#include "ConnectionManagerContext.h"
#include "RateLimiterContext.h"

#include "events/ReceiveEvent.h"
#include "events/SendEvent.h"
#include "events/CloseEvent.h"
#include "events/NetworkReceivePacketEvent.h"

using namespace std;

class SimpleTCPIContextContainer : public gc {
public:
    SimpleTCPIContextContainer();

    SimpleTCPIContextContainer(SimpleTCPIContextContainer const& other);

    IContext* get_congestion_control();

    IContext* get_reliability();

    IContext* get_connection_manager();

    IContext* get_rate_limiter();

    SendEvent* get_saved_send_event();

    void set_saved_send_event(SendEvent* e);

    ReceiveEvent* get_saved_receive_event();

    void set_saved_receive_event(ReceiveEvent* e);

    CloseEvent* get_saved_close_event();

    void set_saved_close_event(CloseEvent* e);

    NetworkReceivePacketEvent* get_fin();

    void set_fin(NetworkReceivePacketEvent* e);

    u_int32_t get_echo_reply();

    void set_echo_reply(u_int32_t echo_reply);

private:
    IContext* reliability_;
    IContext* cc_;
    IContext* cm_;
    IContext* rl_;

    SendEvent* saved_send_event_;
    ReceiveEvent* saved_receive_event_;
    CloseEvent* close_event_;
    NetworkReceivePacketEvent* fin_;

    u_int32_t echo_reply_;
};

#endif	/* CONTEXTCONTAINER_H */

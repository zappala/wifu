/* 
 * File:   SimpleUDPContainer.h
 * Author: rbuck
 *
 * Created on January 11, 2012, 9:57 PM
 */

#ifndef SIMPLEUDPCONTAINER_H
#define	SIMPLEUDPCONTAINER_H

#include "events/framework_events/NetworkReceivePacketEvent.h"
#include "events/framework_events/ReceiveEvent.h"
#include <queue>
#include <deque>

#define packet_queue queue<NetworkReceivePacketEvent*, deque<NetworkReceivePacketEvent*, gc_allocator<NetworkReceivePacketEvent*> > >

class SimpleUDPContainer : public gc {
private:
     packet_queue packet_queue_;
     ReceiveEvent* receive_event_;

public:

    SimpleUDPContainer();
    virtual ~SimpleUDPContainer();

    void set_receive_event(ReceiveEvent* e);
    ReceiveEvent* get_receive_event() const;

    packet_queue& get_packet_queue();
    
};

#endif	/* SIMPLEUDPCONTAINER_H */


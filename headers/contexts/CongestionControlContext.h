/* 
 * File:   CongestionControlContext.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 10:37 AM
 */

#ifndef CONGESTIONCONTROLCONTEXT_H
#define	CONGESTIONCONTROLCONTEXT_H

#include <string>
#include "Context.h"
#include "states/SlowStart.h"
#include "AddressPort.h"
#include "Socket.h"
#include "packet/WiFuPacket.h"
#include "events/ConnectEvent.h"
#include "events/AcceptEvent.h"
#include "events/TimerFiredEvent.h"

#include "IQueue.h"
#include "Queue.h"

using namespace std;

class CongestionControlContext : public Context {
public:
    CongestionControlContext();
    virtual ~CongestionControlContext();

    IQueue<unsigned char>& get_queue();
    int get_max_buffer_size();
    bool can_send_data();
    void set_can_send_data(bool can_send);

private:

    Queue<unsigned char> buffer_;
    int max_buffer_size_;
    bool can_send_;

};

#endif	/* CONGESTIONCONTROLCONTEXT_H */

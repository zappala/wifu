/* 
 * File:   SendRateLimiter.cc
 * Author: leer
 * 
 * Created on April 21, 2011, 3:38 PM
 */

#include "states/tcp-ap/SendRateLimiter.h"

SendRateLimiter::SendRateLimiter() : canSend_(1), sendSeconds_(0), sendNanos_(0), waitingTimeout_(0) {
    queue_ = new Queue<WiFuPacket*>();
}

SendRateLimiter::~SendRateLimiter() {
    delete queue_;
}

//rate of 1 packet per (seconds + nanoseconds)
void SendRateLimiter::setRate(int seconds,  long int nanoseconds)
{
    sendSeconds_ = seconds;
    sendNanos_ = nanoseconds;
}

int SendRateLimiter::getSeconds()
{
    return sendSeconds_;
}

long int SendRateLimiter::getNanoseconds()
{
    return sendNanos_;
}

void SendRateLimiter::state_send_packet(Context* c, SendPacketEvent* e)
{
    //cout << "SendRateLimiter::send_packet: Entered\n";
    if(canSend_)
    {
        //cout << "SendRateLimiter::send_packet: Sending packet: " << e->get_packet() << "\n";
        canSend_ = 0;
        //dispatch NetworkSendPacketEvent, etc.
        NetworkSendPacketEvent* netSendEvent = new NetworkSendPacketEvent(e->get_socket(), e->get_packet());
        Dispatcher::instance().enqueue(netSendEvent);
        setTimer(e->get_socket());
    }
    else
    {
        //cout << "SendRateLimiter::send_packet: Enqueuing packet: " << e->get_packet() << "\n";
        //enqueue to send later
        queue_->enqueue(e->get_packet());
        //cout << "Queue: " + queue_->size() << "\n";
    }
}

void SendRateLimiter::state_timer_fired(Context* c, TimerFiredEvent* e)
{
    //cout << "SendRateLimiter::timer_fired: got timer: " << e->get_timeout_event() << endl;
    //make sure this is ours
    if(waitingTimeout_ == 0 || e->get_timeout_event() != waitingTimeout_)
    {
        //cout << "SendRateLimiter::timer_fired: got timer: " << e->get_timeout_event() << " but wanted " << waitingTimeout_ << endl;
        return;
    }

    //if data to send, dispatch the NetworkSendPacketEvent
    if(!queue_->isEmpty())
    {
        cout << "SendRateLimiter::timer_fired: sending queued data: " << endl;
        //send packet
        WiFuPacket* packet = queue_->dequeue();
        cout << "SendRateLimiter::timer_fired: data to send: " << packet << endl;
        NetworkSendPacketEvent* netSendEvent = new NetworkSendPacketEvent(e->get_socket(), packet);
        Dispatcher::instance().enqueue(netSendEvent);
        setTimer(e->get_socket());
    }
    else
    {
        canSend_ = 1;
        waitingTimeout_= 0;
    }
}

void SendRateLimiter::setTimer(Socket* s)
{
    TimeoutEvent* timeout_event = new TimeoutEvent(s, sendSeconds_, sendNanos_);
    waitingTimeout_ = timeout_event;
    Dispatcher::instance().enqueue(timeout_event);
}


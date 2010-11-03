/* 
 * File:   QConnectionManager.h
 * Author: rbuck
 *
 * Created on October 29, 2010, 2:12 PM
 */

#ifndef _CONNECTIONMANAGER_H
#define	_CONNECTIONMANAGER_H

#include "Module.h"
#include "ConnectEvent.h"
#include "SendPacketEvent.h"
#include "TimeoutEvent.h"

class ConnnectionManager : public Module {
public:

    ConnnectionManager() : Module() {

    }

    virtual ~ConnnectionManager() {
        cout << "";
    }

    void connect(Event* e) {
        ConnectEvent* c = (ConnectEvent*) e;
        cout << "Connection Manager Connect: " << c->get_address() << " " << c->get_port() << endl;
        dispatch(new SendPacketEvent(c->get_socket(), c->get_address(), c->get_port()));

        // Timeout in 1 sec
        timer_ = new TimeoutEvent(e->get_socket(), 1, 0);
        dispatch_timeout(timer_);
    }

    void data(Event* e) {

    }

    void receive(Event* e) {
        cout << "Received Response" << endl;
        cancel_timeout(timer_);
    }

    void my_timer_fired(Event* e) {
        TimerFiredEvent* event = (TimerFiredEvent*) e;
        cout << "Timer Fired: Seconds: " << event->get_timeout_event()->get_timeout_time().tv_sec;
        cout << " Nanoseconds: " << event->get_timeout_event()->get_timeout_time().tv_nsec << endl;
    }

    void test() {
        int socket = 1;
        TimeoutEvent * one = new TimeoutEvent(socket, 1, 0);
        TimeoutEvent * oneplus = new TimeoutEvent(socket, 1, 0);
        TimeoutEvent * six = new TimeoutEvent(socket, 6, 0);
        TimeoutEvent * two = new TimeoutEvent(socket, 2, 0);
        TimeoutEvent * three = new TimeoutEvent(socket, 3, 0);
        TimeoutEvent * four = new TimeoutEvent(socket, 4, 0);
        TimeoutEvent * five = new TimeoutEvent(socket, 5, 0);


        dispatch_timeout(one);
        dispatch_timeout(oneplus);
        dispatch_timeout(two);
        dispatch_timeout(six);
        dispatch_timeout(four);
        dispatch_timeout(five);
        dispatch_timeout(three);

        cancel_timeout(two);
    }
private:
    TimeoutEvent* timer_;
};


#endif	/* _CONNECTIONMANAGER_H */


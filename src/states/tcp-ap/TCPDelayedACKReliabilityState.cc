#include "states/tcp-ap/TCPDelayedACKReliabilityState.h"

TCPDelayedACKReliabilityState::TCPDelayedACKReliabilityState() : TCPTahoeReliabilityState() {
}

TCPDelayedACKReliabilityState::~TCPDelayedACKReliabilityState() {

}

void TCPDelayedACKReliabilityState::state_timer_fired(Context* c, QueueProcessor<Event*>* q, TimerFiredEvent* e) {
    cout << "TCPDelayedACKReliabilityState::state_timer_fired() on socket: " << e->get_socket() << endl;
    TCPDelayedACKReliabilityContext* rc = (TCPDelayedACKReliabilityContext*) c;
    Socket* s = e->get_socket();

    if (rc->get_timeout_event() == e->get_timeout_event()) {
        rc->set_rto(rc->get_rto() * 2);
        resend_data(c, q, s);
    }
    else if (rc->get_ack_timeout_event() == e->get_timeout_event()) {
        //TODO: force sending an ACK
        cout << "TCPDelayedACKReliabilityState::state_timer_fired(): sending delayed ACK\n";
        create_and_dispatch_ack(q, s);
    }
}

void TCPDelayedACKReliabilityState::start_ack_timer(Context* c, Socket* s) {
    TCPDelayedACKReliabilityContext* rc = (TCPDelayedACKReliabilityContext*) c;
    cout << "TCPDelayedACKReliabilityState::start_ack_timer() on socket: " << s << endl;
    cout << "TCPDelayedACKReliabilityState::start_ack_timer() using timeout value: " << rc->get_delay_timeout_interval() << "\n";
    // only start the timer if it is not already running
    if (!rc->get_timeout_event()) {
        double seconds;
        long int nanoseconds = modf(rc->get_delay_timeout_interval(), &seconds) * NANOSECONDS_IN_SECONDS;
        TimeoutEvent* timer = new TimeoutEvent(s, seconds, nanoseconds);
        rc->set_ack_timeout_event(timer);
        Dispatcher::instance().enqueue(timer);
    }
}

//Do we need to reset the ACK timer?
//void TCPDelayedACKReliabilityState::reset_ack_timer(Context* c, Socket* s) {
//    //    cout << "TCPDelayedACKReliabilityState::reset_timer() on socket: " << s << endl;
//    cancel_timer(c, s);
//    start_timer(c, s);
//}

void TCPDelayedACKReliabilityState::cancel_ack_timer(Context* c, Socket* s) {
    //    cout << "TCPDelayedACKReliabilityState::cancel_timer() on socket: " << s << endl;
    TCPDelayedACKReliabilityContext* rc = (TCPDelayedACKReliabilityContext*) c;

    assert(rc->get_ack_timeout_event());
    //    cout << "TCPDelayedACKReliabilityState::cancel_timer(): " << rc->get_timeout_event() << endl;
    CancelTimerEvent* event = new CancelTimerEvent(rc->get_ack_timeout_event());
    Dispatcher::instance().enqueue(event);
    rc->set_ack_timeout_event(0);
}

void TCPDelayedACKReliabilityState::handle_data(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    TCPDelayedACKReliabilityContext* rc = (TCPDelayedACKReliabilityContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();
    Socket* s = e->get_socket();

    int num_inserted = rc->get_receive_window().insert(p);
    if (rc->get_rcv_wnd() - num_inserted >= 0) {

        rc->set_rcv_wnd(rc->get_rcv_wnd() - num_inserted);
        string& receive_buffer = s->get_receive_buffer();
        u_int32_t before_rcv_buffer_size = receive_buffer.size();
        rc->get_receive_window().get_continuous_data(rc->get_rcv_nxt(), receive_buffer);
        u_int32_t after_receive_buffer_size = receive_buffer.size();
        u_int32_t amount_put_in_receive_buffer = after_receive_buffer_size - before_rcv_buffer_size;
        assert(amount_put_in_receive_buffer >= 0);

        if (amount_put_in_receive_buffer > 0) {
            rc->set_rcv_nxt(rc->get_rcv_nxt() + amount_put_in_receive_buffer);
            q->enqueue(new ReceiveBufferNotEmptyEvent(s));
        }
    } else {
        // I don't think we should get here.
        // I ran some tests with the assert on and it never asserted.
        // But just in case...
        //assert(false);
        rc->get_receive_window().remove(p);
    }

    //CHANGE FROM TCP:
    //We delay our ACKs based on delay_count_ OR a timeout value.

    cout << "TCPDelayedACKReliabilityState::handle_data(): entering Delayed ACK section\n";
    cur_ack_count_ += 1;

    //Just make sure we're using a delay.
    if(rc->get_delay_count() <= 0) {
        create_and_dispatch_ack(q, s);
        return;
    }
    cout << "TCPDelayedACKReliabilityState::handle_data(): using a delay of " << rc->get_delay_count() << "ACKs.\n";
    //we have enough data packets, send an an ACK
    if(cur_ack_count_ >= rc->get_delay_count()){
        cout << "TCPDelayedACKReliabilityState::handle_data(): count reached, sending ACK\n";
        create_and_dispatch_ack(q, s);
        //reset our local count
        cur_ack_count_ = 0;
        cancel_ack_timer(c, s);
    }
    //Check to see if we have a timer going; if not, we'll need one now
    else if(rc->get_ack_timeout_event() == 0) {
        cout << "TCPDelayedACKReliabilityState::handle_data(): starting ACK timer\n";
        start_ack_timer(c, s);
    }
}

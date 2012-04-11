#include "states/tcp-ap/TCPDelayedACKReliabilityState.h"

TCPDelayedACKReliabilityState::TCPDelayedACKReliabilityState() : TCPTahoeReliabilityState() {
}

TCPDelayedACKReliabilityState::~TCPDelayedACKReliabilityState() {

}

void TCPDelayedACKReliabilityState::state_timer_fired(Context* c, QueueProcessor<Event*>* q, TimerFiredEvent* e) {
    cout << "TCPDelayedACKReliabilityState::state_timer_fired() on socket: " << e->get_socket() << endl;
    TCPDelayedACKReliabilityContext* rc = (TCPDelayedACKReliabilityContext*) c;
    Socket* s = e->get_socket();

    this->TCPTahoeReliabilityState::state_timer_fired(c, q, e);

    if (rc->get_ack_timeout_event() == e->get_timeout_event()) {
        //force sending an ACK
        cout << "TCPDelayedACKReliabilityState::state_timer_fired(): sending delayed ACK\n";
        //TODO: Should we send an ACK immediately, or should we aggregate this with the rest?
        rc->set_ack_timeout_event(0);
        rc->set_cur_ack_count(0);
        this->TCPTahoeReliabilityState::create_and_dispatch_ack(c, q, e);
    }
}

void TCPDelayedACKReliabilityState::start_ack_timer(Context* c, Socket* s) {
    TCPDelayedACKReliabilityContext* rc = (TCPDelayedACKReliabilityContext*) c;
    cout << "TCPDelayedACKReliabilityState::start_ack_timer() on socket: " << s << endl;
    //cout << "TCPDelayedACKReliabilityState::start_ack_timer() using timeout value: " << rc->get_delay_timeout_interval() << "\n";
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
    
    //Make sure we figure out the correct delay for this packet
    rc->set_delay_count(get_delay_based_on_seq_num(rc, p->get_tcp_sequence_number()));

    //Do all the regular stuff
    this->TCPTahoeReliabilityState::handle_data(c, q, e);
}

void TCPDelayedACKReliabilityState::create_and_dispatch_ack(Context* c, QueueProcessor<Event*>* q, Event* e) {
    TCPDelayedACKReliabilityContext* rc = (TCPDelayedACKReliabilityContext*) c;

    //TCPPacket* p = (TCPPacket*) e->get_packet();
    Socket* s = e->get_socket();

    //CHANGE FROM TCP:
    //We delay our ACKs based on delay_count_ OR a timeout value.

    cout << "TCPDelayedACKReliabilityState::handle_data(): entering Delayed ACK section\n";
    rc->set_cur_ack_count(rc->get_cur_ack_count() + 1);

    //Just make sure we're using a delay.
    /*if(rc->get_delay_count() <= 0) {
        create_and_dispatch_ack(c, q ,e);
        return;
    }*/

    cout << "TCPDelayedACKReliabilityState::create_and_dispatch_ack(): using a delay of " << rc->get_delay_count() << "ACKs.\n";
    cout << "TCPDelayedACKReliabilityState::create_and_dispatch_ack(): currently have " << rc->get_cur_ack_count() << "\n";
    //we have enough data packets, send an an ACK
    if(rc->get_cur_ack_count() >= rc->get_delay_count()){
        cout << "TCPDelayedACKReliabilityState::create_and_dispatch_ack(): count reached, sending ACK\n";

        if(rc->get_delay_count() > 1){
            cancel_ack_timer(c, s);
        }
        //reset our local count
        rc->set_cur_ack_count(0);
        this->TCPTahoeReliabilityState::create_and_dispatch_ack(c, q, e);
    }
    //Check to see if we have a timer going; if not, we'll need one now
    else if(rc->get_ack_timeout_event() == 0) {
        cout << "TCPDelayedACKReliabilityState::create_and_dispatch_ack(): starting ACK timer\n";
        start_ack_timer(c, s);
    }
}

u_int16_t TCPDelayedACKReliabilityState::get_delay_based_on_seq_num(Context* c, u_int32_t seqnum) {
    TCPDelayedACKReliabilityContext* rc = (TCPDelayedACKReliabilityContext*) c;

    cout << "get_delay_based_on_seq_num(): seqnum is " << seqnum << endl;

    if(seqnum < rc->get_l1_threshold()){
        return rc->get_delay1();
    }
    if(seqnum >= rc->get_l1_threshold() && seqnum < rc->get_l2_threshold()){
        return rc->get_delay2();
    }
    if(seqnum >= rc->get_l2_threshold() && seqnum < rc->get_l3_threshold()){
        return rc->get_delay3();
    }
    if(seqnum >= rc->get_l3_threshold()){
        return rc->get_delay4();
    }
}

void TCPDelayedACKReliabilityState::state_set_socket_option(Context* c, QueueProcessor<Event*>* q, SetSocketOptionEvent* e) {
    TCPDelayedACKReliabilityContext* rc = (TCPDelayedACKReliabilityContext*) c;
    cout << "TCPDelayedACKReliabilityState::state_set_socket_option()" << endl;
    if(e->get_level() == TCP_DELAYEDACK) {
        Socket* s = e->get_socket();
        //l1
        if(e->get_option_name() == 1) {
            pair<int,int>* option_name = new pair<int, int>(TCP_DELAYEDACK, 1);
            int l1 = *(int*) (s->get_socket_options().get(*option_name).first.data());

            cout << "TCPDelayedACKReliabilityState::state_set_socket_option(): setting l1 to " << l1 << endl;
            rc->set_l1_threshold(l1);
        }
        //l2
        else if(e->get_option_name() == 2) {
            pair<int,int>* option_name = new pair<int, int>(TCP_DELAYEDACK, 2);
            int l2 = *(int*) (s->get_socket_options().get(*option_name).first.data());

            cout << "TCPDelayedACKReliabilityState::state_set_socket_option(): setting l2 to " << l2 << endl;
            rc->set_l2_threshold(l2);
        }
        //l3
        else if(e->get_option_name() == 3) {
            pair<int,int>* option_name = new pair<int, int>(TCP_DELAYEDACK, 3);
            int l3 = *(int*) (s->get_socket_options().get(*option_name).first.data());

            cout << "TCPDelayedACKReliabilityState::state_set_socket_option(): setting l3 to " << l3 << endl;
            rc->set_l3_threshold(l3);
        }
    }
}

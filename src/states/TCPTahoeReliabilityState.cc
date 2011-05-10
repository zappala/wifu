#include "states/TCPTahoeReliabilityState.h"
#include "events/CancelTimerEvent.h"
#include "packet/TCPPacket.h"

TCPTahoeReliabilityState::TCPTahoeReliabilityState() {

}

TCPTahoeReliabilityState::~TCPTahoeReliabilityState() {

}

void TCPTahoeReliabilityState::state_send_packet(Context* c, SendPacketEvent* e) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    Socket* s = e->get_socket();


    // RTT calculations
    if (!rc->get_timer()) {
        // Timer not set
        TimeoutEvent* timeout_event = new TimeoutEvent(s, 0, rc->get_rto());
        rc->set_timer(timeout_event);
        Dispatcher::instance().enqueue(timeout_event);
    }

}

void TCPTahoeReliabilityState::state_receive_packet(Context* c, NetworkReceivePacketEvent* e) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    Socket* s = e->get_socket();
    TCPPacket* p = (TCPPacket*) e->get_packet();

    if (p->is_tcp_ack()) {
        Dispatcher::instance().enqueue(new CancelTimerEvent(rc->get_timer()));
        rc->set_timer(0);

        //TODO: remove data from resend buffer, if any

        // if there is still unacknowleged data
        if (!s->get_resend_buffer().empty()) {
            TimeoutEvent* timeout_event = new TimeoutEvent(s, 0, rc->get_rto());
            rc->set_timer(timeout_event);
            Dispatcher::instance().enqueue(timeout_event);
        }
    }
}

void TCPTahoeReliabilityState::state_timer_fired(Context* c, TimerFiredEvent* e) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    Socket* s = e->get_socket();
    
    TimeoutEvent* timeout_event = rc->get_timer();
    if (timeout_event && timeout_event == e->get_timeout_event()) {
        // valid timeout
        rc->set_timer(0);
        
        // Retransmit the earliest segment that has not been acknowledged by the TCP receiver.
        rc->set_rto(rc->get_rto() * 2);

        TimeoutEvent* timeout_event = new TimeoutEvent(s, 0, rc->get_rto());
        rc->set_timer(timeout_event);
        Dispatcher::instance().enqueue(timeout_event);
    }
}
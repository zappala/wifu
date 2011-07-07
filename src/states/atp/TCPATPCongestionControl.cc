/*
 * TCPATPCongestionControl.cc
 *
 *  Created on: Jun 23, 2011
 *      Author: philipbl
 */

#include "states/atp/TCPATPCongestionControl.h"

// must be here to avoid circular dependencies
#include "contexts/ATPCongestionControlContext.h"


//#define PRINT_DEBUG
//#define PRINT_PACKET

#ifdef PRINT_DEBUG
	#define DEBUG(str) cout << str << endl;
#else
	#define DEBUG(str)
#endif

#ifdef PRINT_PACKET
	#define DEBUG_PACKET(str) cout << str << endl;
#else
	#define DEBUG_PACKET(str)
#endif

TCPATPCongestionControl::TCPATPCongestionControl() : canSend_(1), sendSeconds_(0), sendMicros_(0), waitingTimeout_(0) {
    send_queue_ = new Queue<WiFuPacket*>();
}

TCPATPCongestionControl::~TCPATPCongestionControl() {
    delete send_queue_;
}

//rate of 1 packet per (seconds + nanoseconds)

void TCPATPCongestionControl::setDelay(int seconds, long int microseconds) {
	DEBUG("Delay getting set to: " << seconds << ", " << microseconds);

	//assert(seconds != 0 || microseconds != 0);

	if(seconds == 0 && microseconds == 0){
		cout << "WARNING: seconds and microseconds is 0!" << endl;
	}

	cout << "\t\tSetting rate to: " << seconds << ", " << microseconds << endl;

    sendSeconds_ = seconds;
    sendMicros_ = microseconds;
}

int TCPATPCongestionControl::getSeconds() {
    return sendSeconds_;
}

long int TCPATPCongestionControl::getMicroseconds() {
    return sendMicros_;
}

void TCPATPCongestionControl::state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e) {
    DEBUG("TCPATPCongestionControl::state_send_packet : rate = " << sendSeconds_ << ", " << sendMicros_);

    if (canSend_) {
        canSend_ = 0;


		ATPPacket* packet = ATPPacket::convert_to_atp_packet(e->get_packet());
		assert(packet != 0);

        // Need to update the time stamp before sending it out
		TCPTimestampOption * option = (TCPTimestampOption *)packet->get_option(8);
        option->set_timestamp();

        DEBUG("TCPATPCongestionControl::send_packet: Sending packet");
        send_packet(packet, e->get_socket());


        setTimer(e->get_socket());
    }
    else {
    	DEBUG("TCPATPCongestionControl::state_send_packet : adding another to packet to queue (" << send_queue_->size() << ")");
        DEBUG("\t with data: " << e->get_packet()->get_data());
    	// enqueue to send later
        send_queue_->enqueue(e->get_packet());
    }
}

void TCPATPCongestionControl::state_resend_packet(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e){
    ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;

    DEBUG("TCPATPCongestionControl::state_resend_packet");

	ccc->set_snd_nxt(ccc->get_snd_una());

	// Clear out old queue since I will need to resend from UNA
	send_queue_->clear();

	// resends SYN, ACKSYN, FIN packets
    resend_data(c, q, e);

    // Fill queue back up with data from buffer
    send_packets(c, q, e);
}

void TCPATPCongestionControl::state_timer_fired(Context* c, QueueProcessor<Event*>* q, TimerFiredEvent* e) {
    //cout << "TCPATPCongestionControl::timer_fired: got timer" << endl;

    //make sure this is ours
    if (waitingTimeout_ == 0 || e->get_timeout_event() != waitingTimeout_) {
        DEBUG("TCPATPCongestionControl::timer_fired: not my timer");
        return;
    }

    //if data to send, dispatch the NetworkSendPacketEvent
    if (!send_queue_->is_empty()) {
        DEBUG("SendRateLimiter::timer_fired: sending queued data ");

        //send packet

    	ATPPacket* packet = ATPPacket::convert_to_atp_packet(send_queue_->dequeue());
    	assert(packet != 0);

    	send_packet(packet, e->get_socket());
        setTimer(e->get_socket());
    }
    else {
    	DEBUG("SendRateLimiter::timer_fired: nothing to send ");
        canSend_ = 1;
        waitingTimeout_ = 0;
    }
}

void TCPATPCongestionControl::setTimer(Socket* s) {
	// converting sendMicros_ into nanoseconds
	TimeoutEvent* timeout_event = new TimeoutEvent(s, sendSeconds_, sendMicros_ * 1000);
    waitingTimeout_ = timeout_event;
    Dispatcher::instance().enqueue(timeout_event);
}

void TCPATPCongestionControl::send_packet(ATPPacket* p, Socket* s){

	DEBUG("SENDING DATA: " << p->get_data());
    p->pack();
    p->calculate_and_set_tcp_checksum();

    printPacket(p);

    NetworkSendPacketEvent* netSendEvent = new NetworkSendPacketEvent(s, p);
    Dispatcher::instance().enqueue(netSendEvent);
}

void TCPATPCongestionControl::send_packets(Context* c, QueueProcessor<Event*>* q, Event* e) {
	//cout << "TCPATPCongestionControl::send_packets" << endl;

	ATPCongestionControlContext* ccc = (ATPCongestionControlContext* )c;
	Socket* s = e->get_socket();
    string& send_buffer = s->get_send_buffer();

	DEBUG("TCPATPCongestionControl::send_packets : sending packets:");

	while (((int)send_buffer.size() > (int)ccc->get_num_outstanding())) {
    	delay_send_packets(c, q, e);
    }
}

void TCPATPCongestionControl::delay_send_packets(Context* c, QueueProcessor<Event*>* q, Event* e) {
	//cout << "TCPATPCongestionControl::delay_send_packet()" << endl;

	ATPCongestionControlContext* ccc = (ATPCongestionControlContext* )c;
	Socket* s = e->get_socket();

	string& send_buffer = s->get_send_buffer();

	// Represents the first byte in the send buffer which has "never" been sent before.
	// We actually may have sent data already but if SND.UNA changes due to a drop we will treat it as if we never sent it.
	int index = ccc->get_num_outstanding();

	//cout << "UNA: " << ccc->get_snd_una() << endl;
	//cout << "NXT: " << ccc->get_snd_nxt() << endl;

	ATPPacket* p = new ATPPacket();
	p->insert_tcp_header_option(new TCPTimestampOption());

	int data_length = get_send_data_length(c, e, p);
	assert(data_length > 0);

	const char* data = (send_buffer.data() + index);

	ccc->set_snd_nxt(ccc->get_snd_nxt() + data_length);

	AddressPort* destination = s->get_remote_address_port();
	AddressPort* source = s->get_local_address_port();

	p->set_ip_destination_address_s(destination->get_address());
	p->set_destination_port(destination->get_port());

	p->set_ip_source_address_s(source->get_address());
	p->set_source_port(source->get_port());

	p->set_data((unsigned char*) data, data_length);
	assert(p->get_data_length_bytes() > 0);

	//cout << "TCPATPCongestionControl::delay_send_packets : data being sent: " << p->get_data() << endl;
	DEBUG("UNA: " << ccc->get_snd_una());
	DEBUG("NXT: " << ccc->get_snd_nxt());

	q->enqueue(new SendPacketEvent(s, p));
}


void TCPATPCongestionControl::resend_data(Context* c, QueueProcessor<Event*>* q, Event* e) {
    DEBUG("TCPTahoeBaseCongestionControl::resend_data()");
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    Socket* s = e->get_socket();

    TCPPacket* p = new ATPPacket();
    p->insert_tcp_header_option(new TCPTimestampOption());

    AddressPort* destination = s->get_remote_address_port();
    AddressPort* source = s->get_local_address_port();

    p->set_ip_destination_address_s(destination->get_address());
    p->set_destination_port(destination->get_port());

    p->set_ip_source_address_s(source->get_address());
    p->set_source_port(source->get_port());

    // Check for SYN or FIN byte in the buffer
    string& send_buffer = s->get_send_buffer();
    assert(!send_buffer.empty());
    bool control_bit = false;

    if (!send_buffer.compare(0, 1, SYN_BYTE.c_str())) {
        p->set_tcp_syn(true);
        control_bit = true;

    } else if (!send_buffer.compare(0, 1, FIN_BYTE.c_str())) {
        p->set_tcp_fin(true);
        control_bit = true;
    }

    if (control_bit) {
        //                cout << "Control bit set, setting snd_nxt to snd.una + 1" << endl;
        p->set_data((unsigned char*) "", 0);
    } /*else {
        // TODO: change this to use the string::data() method instead of substr() so we can avoid the copy
        int length = get_resend_data_length(c, e, p);
        if (!send_buffer.compare(send_buffer.size() - 1, 1, FIN_BYTE.c_str())) {

            length -= 1;
        }
        p->set_data((unsigned char*) send_buffer.data(), length);
        assert(p->get_data_length_bytes() > 0);
    }
	*/
    u_int32_t length = (p->is_tcp_syn() || p->is_tcp_fin()) ? 1 : p->get_data_length_bytes();
    ccc->set_snd_nxt(ccc->get_snd_una() + length);

    SendPacketEvent* event = new SendPacketEvent(s, p);
    q->enqueue(event);
}

void TCPATPCongestionControl::printPacket(ATPPacket* packet){
	DEBUG_PACKET("-------PACKET-------");
	DEBUG_PACKET("SYN: " << packet->is_tcp_syn());
	DEBUG_PACKET("ACK: " << packet->is_tcp_ack());
	DEBUG_PACKET("FIN: " << packet->is_tcp_fin());
	DEBUG_PACKET("SEQ #: " << packet->get_tcp_sequence_number());
	DEBUG_PACKET("ACK #: " << packet->get_tcp_ack_number());
	DEBUG_PACKET("MAX DELAY: " << packet->get_atp_max_delay());
	DEBUG_PACKET("AVG DELAY: " << packet->get_atp_average_delay());
	DEBUG_PACKET("data length: " << packet->get_data_length_bytes());
	DEBUG_PACKET("data: " << packet->get_data());
	DEBUG_PACKET("--------------------");
}

int TCPATPCongestionControl::get_send_data_length(Context* c, Event* e, WiFuPacket* p) {
    ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;
    string& send_buffer = e->get_socket()->get_send_buffer();

    // TODO: Should i really be considering receiver window size? I am delaying sending so who knows
    // what the window size will be when the packet actually gets sent

    // Takes into account receiver window size, packets max size, data in the send buffer
	//return min((int)ccc->get_snd_wnd(), min(p->max_data_length(), (int) send_buffer.size() - (int) ccc->get_num_outstanding()));

    return min(p->max_data_length(), (int) send_buffer.size() - (int) ccc->get_num_outstanding());
}


int TCPATPCongestionControl::get_resend_data_length(Context* c, Event* e, WiFuPacket* p) {
    ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;
    string& send_buffer = e->get_socket()->get_send_buffer();

    // SND.NXT should have previously been set back to SND.UNA
    int num_unsent = (int) send_buffer.size() - (int) ccc->get_num_outstanding();

    // we do not want to make a packet larger than the window size
    int data_length = min(min(num_unsent, (int) p->max_data_length()), min((int) ccc->get_max_allowed_to_send(), MAX_TCP_RECEIVE_WINDOW_SIZE));
    assert(data_length > 0);
    return data_length;
}

void TCPATPCongestionControl::update_context(Context* c, ATPPacket* packet){
	ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;

	ccc->set_snd_una(packet->get_tcp_ack_number());

    // In case we get an ack for something later than snd.nxt
    // (we dropped a packet but subsequent packets got through and we received a cumulative ack)
    if (less_than(ccc->get_snd_nxt(), ccc->get_snd_una())) {
    	DEBUG("TCPATPCongestionControl::update_context : NXT is less than UNA, must have received cum ACK");
        ccc->set_snd_nxt(ccc->get_snd_una());
    }

    // update send window (RFC p. 72)
    if (less_than(ccc->get_snd_wnd1(), packet->get_tcp_sequence_number()) ||
            (ccc->get_snd_wnd1() == packet->get_tcp_sequence_number() &&
            less_than_or_equal(ccc->get_snd_wnd2(), packet->get_tcp_ack_number()))) {

        ccc->set_snd_wnd(packet->get_tcp_receive_window_size());
        ccc->set_snd_wnd1(packet->get_tcp_sequence_number());
        ccc->set_snd_wnd2(packet->get_tcp_ack_number());
    }

    DEBUG("---- updating context ----");
    DEBUG("snd window: " << ccc->get_snd_wnd());
	DEBUG("UNA: " << ccc->get_snd_una());
	DEBUG("NXT: " << ccc->get_snd_nxt());
}

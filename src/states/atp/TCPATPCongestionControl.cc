/*
 * TCPATPCongestionControl.cc
 *
 *  Created on: Jun 23, 2011
 *      Author: philipbl
 */

#include "../../../headers/states/atp/TCPATPCongestionControl.h"

// must be here to avoid circular dependencies
#include "contexts/ATPCongestionControlContext.h"

TCPATPCongestionControl::TCPATPCongestionControl() : canSend_(1), sendSeconds_(0), sendNanos_(0), waitingTimeout_(0) {
    queue_ = new Queue<WiFuPacket*>();
}

TCPATPCongestionControl::~TCPATPCongestionControl() {
    delete queue_;
}

//rate of 1 packet per (seconds + nanoseconds)

void TCPATPCongestionControl::setRate(int seconds, long int nanoseconds) {
	cout << "Rate getting set to: " << seconds << ", " << nanoseconds << endl;
    sendSeconds_ = seconds;
    sendNanos_ = nanoseconds;
}

int TCPATPCongestionControl::getSeconds() {
    return sendSeconds_;
}

long int TCPATPCongestionControl::getNanoseconds() {
    return sendNanos_;
}

void TCPATPCongestionControl::state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e) {
    cout << "TCPATPCongestionControl::send_packet: Entered\n";

    //if (canSend_) {
    //    canSend_ = 0;


		ATPPacket* packet = ATPPacket::convert_to_atp_packet(e->get_packet());
		assert(packet != 0);

        // Need to update the time stamp before sending it out
		TCPTimestampOption * option = (TCPTimestampOption *)packet->get_option(8);
        option->set_timestamp();

        send_packet(packet, e->get_socket());

        cout << "TCPATPCongestionControl::send_packet: Sending packet" << endl;

        setTimer(e->get_socket());
    /*}
    else {
        // enqueue to send later
        queue_->enqueue(e->get_packet());
    }*/
}

void TCPATPCongestionControl::state_resend_packet(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e){
    ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;

	ccc->set_snd_nxt(ccc->get_snd_una());
    resend_data(c, q, e);
}

void TCPATPCongestionControl::state_timer_fired(Context* c, QueueProcessor<Event*>* q, TimerFiredEvent* e) {
    cout << "TCPATPCongestionControl::timer_fired: got timer" << endl;

    //make sure this is ours
    if (waitingTimeout_ == 0 || e->get_timeout_event() != waitingTimeout_) {
        cout << "TCPATPCongestionControl::timer_fired: not my timer" << endl;
        return;
    }

    //if data to send, dispatch the NetworkSendPacketEvent
    if (!queue_->is_empty()) {
        cout << "SendRateLimiter::timer_fired: sending queued data " << endl;

        //send packet
    	ATPPacket* packet = dynamic_cast<ATPPacket *>(queue_->dequeue());
    	assert(packet != 0);

    	send_packet(packet, e->get_socket());
        setTimer(e->get_socket());
    }
    else {
    	cout << "SendRateLimiter::timer_fired: nothing to send " << endl;
        canSend_ = 1;
        waitingTimeout_ = 0;
    }
}

void TCPATPCongestionControl::setTimer(Socket* s) {
    // TODO: Turn timer back on

	//TimeoutEvent* timeout_event = new TimeoutEvent(s, sendSeconds_, sendNanos_);
    //waitingTimeout_ = timeout_event;
    //Dispatcher::instance().enqueue(timeout_event);
}

void TCPATPCongestionControl::send_packet(ATPPacket* p, Socket* s){

    p->pack();
    p->calculate_and_set_tcp_checksum();

	//TCPTimestampOption* ts = (TCPTimestampOption*) p->get_option(TCPOPT_TIMESTAMP);
	//cout << ts->get_echo_reply() << endl;

    printPacket(p);

    NetworkSendPacketEvent* netSendEvent = new NetworkSendPacketEvent(s, p);
    Dispatcher::instance().enqueue(netSendEvent);
}

void TCPATPCongestionControl::send_packets(Context* c, QueueProcessor<Event*>* q, Event* e) {
	cout << "TCPATPCongestionControl::send_packets" << endl;

	ATPCongestionControlContext* ccc = (ATPCongestionControlContext* )c;
	Socket* s = e->get_socket();
    string& send_buffer = s->get_send_buffer();

    // TODO: Do I send as much data as I can per packet?
    //			-> No! Send what the receiver is able to receive
    // TODO: Do I need to keep track of what data I have already sent?

    cout << "Send buffer size: " << send_buffer.size() << endl;
    cout << "Number of outstanding packets: " << ccc->get_num_outstanding() << endl;
    cout << "Receiver window size: " << ccc->get_receiver_window_size() << endl;

    while (((int)send_buffer.size() > (int)ccc->get_num_outstanding()) && ((int)ccc->get_receiver_window_size() > (int) ccc->get_num_outstanding())) {
    	cout << "TCPATPCongestionControl::send_packets : sending delay packet" << endl;
    	delay_send_packets(c, q, e);
    }
}

void TCPATPCongestionControl::delay_send_packets(Context* c, QueueProcessor<Event*>* q, Event* e) {
	cout << "TCPATPCongestionControl::delay_send_packet()" << endl;

	ATPCongestionControlContext* ccc = (ATPCongestionControlContext* )c;
	Socket* s = e->get_socket();

	string& send_buffer = s->get_send_buffer();

	// Represents the first byte in the send buffer which has "never" been sent before.
	// We actually may have sent data already but if SND.UNA changes due to a drop we will treat it as if we never sent it.
	int index = ccc->get_num_outstanding();

	ATPPacket* p = new ATPPacket();
	p->insert_tcp_header_option(new TCPTimestampOption());

	// TODO: Make sure this is correct
	// packet, window, buffer
	int data_length = min(min((int)ccc->get_receiver_window_size() - index, (int)p->max_data_length()), (int)send_buffer.size() - index);

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

	q->enqueue(new SendPacketEvent(s, p));
}

void TCPATPCongestionControl::resend_data(Context* c, QueueProcessor<Event*>* q, Event* e) {
    cout << "TCPTahoeBaseCongestionControl::resend_data()" << endl;
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
    } else {
        // TODO: change this to use the string::data() method instead of substr() so we can avoid the copy
        int length = get_resend_data_length(c, e, p);
        if (!send_buffer.compare(send_buffer.size() - 1, 1, FIN_BYTE.c_str())) {

            length -= 1;
        }
        p->set_data((unsigned char*) send_buffer.data(), length);
        assert(p->get_data_length_bytes() > 0);
    }

    u_int32_t length = (p->is_tcp_syn() || p->is_tcp_fin()) ? 1 : p->get_data_length_bytes();
    ccc->set_snd_nxt(ccc->get_snd_una() + length);

    SendPacketEvent* event = new SendPacketEvent(s, p);
    q->enqueue(event);
}

void TCPATPCongestionControl::printPacket(ATPPacket* packet){
	cout << "-------PACKET-------" << endl;
	cout << "SYN: " << packet->is_tcp_syn() << endl;
	cout << "ACK: " << packet->is_tcp_ack() << endl;
	cout << "FIN: " << packet->is_tcp_fin() << endl;
	cout << "SEQ: " << packet->get_tcp_sequence_number() << endl;
	cout << "MAX DELAY: " << packet->get_atp_max_delay() << endl;
	cout << "AVG DELAY: " << packet->get_atp_average_delay() << endl;
	cout << "data length: " << packet->get_data_length_bytes() << endl;
	cout << "data: " << packet->get_data() << endl;
	cout << "--------------------" << endl;
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

void TCPATPCongestionControl::hexDump (char *desc, void *addr, int len) {
    int i;
    unsigned char buff[17];       // stores the ASCII data
    unsigned char *pc = (unsigned char*)addr;     // cast to make the code cleaner.

    // Output description if given.
    if (desc != NULL)
        printf ("%s:\n", desc);

    // Process every byte in the data.
    for (i = 0; i < len; i++) {

    	// Multiple of 16 means new line (with line offset).
        if ((i % 16) == 0) {

        	// Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}

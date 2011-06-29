/*
 * Sender.cc
 *
 *  Created on: Jun 17, 2011
 *      Author: philipbl
 */

#include "../../../headers/states/atp/Sender.h"

Sender::Sender() {
}

Sender::~Sender() {
}

void Sender::state_enter(Context* c){
	ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;

	u_int32_t average_delay = ccc->get_average_delay();

	// Since the SYN + ACK probe was received, just set rate to delay
	// TODO: make sure units are correct here
	setRate(0, average_delay);
}

void Sender::state_exit(Context* c){

}

void Sender::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e){
	cout << "Sender::state_receive_packet: entered" << endl;

	ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;

	ATPPacket* packet = dynamic_cast<ATPPacket *>(e->get_packet());
	assert(packet != 0);

	cout << "Sender::state_receive_packet:" << endl;
	printPacket(packet);

	// Don't care about the packet if it is not an ACK
	// TODO: Maybe I want to know the delay of FIN packets
	if(!packet->is_tcp_ack() || !between_equal_right(ccc->get_snd_una(), packet->get_tcp_ack_number(), ccc->get_snd_max())){
		cout << "Sender::state_receive_packet: didn't receive an ACK or it is out of order" << endl;
		return;
	}

	// If for some reason a SYN probe gets sent out again, it will get taken care of
	if(packet->is_tcp_syn()){
		cout << "Sender::state_receive_packet: received a SYN + ACK, starting over" << endl;

		ccc->set_average_delay(packet->get_atp_average_delay());
		state_enter(ccc);
		return;
	}

	//cout << "Sender::state_receive_packet : updating window size" << endl;
	//update_send_window(c, packet);


	ccc->set_snd_una(packet->get_tcp_ack_number());

	ccc->set_receiver_window_size(packet->get_tcp_receive_window_size());

	update_sending_rate(c, packet);

}

void Sender::state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e){
	cout << "Sender::state_send_packet: entered" << endl;

	ATPPacket* packet = ATPPacket::convert_to_atp_packet(e->get_packet());
	assert(packet != 0);

	super::state_send_packet(c, q, e);
}

void Sender::state_send_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e) {
	cout << "Sender::state_send_buffer_not_empty" << endl;
	send_packets(c, q, e);
}

void Sender::state_resend_packet(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e) {
	//send_packets(c, q, e);
}

void Sender::update_send_window(Context * c, ATPPacket * p){
	ATPCongestionControlContext* ccc = (ATPCongestionControlContext* )c;

    // update send window (RFC p. 72)
    if (less_than(ccc->get_snd_wnd1(), p->get_tcp_sequence_number()) ||
            (ccc->get_snd_wnd1() == p->get_tcp_sequence_number() &&
            less_than_or_equal(ccc->get_snd_wnd2(), p->get_tcp_ack_number()))) {

        ccc->set_snd_wnd(p->get_tcp_receive_window_size());
        ccc->set_snd_wnd1(p->get_tcp_sequence_number());
        ccc->set_snd_wnd2(p->get_tcp_ack_number());
    }
}

void Sender::update_sending_rate(Context * c, ATPPacket * p){
	ATPCongestionControlContext* ccc = (ATPCongestionControlContext* )c;

	u_int32_t average_delay = ccc->get_average_delay();
	long int current_delay = getNanoseconds();

	cout << "Average delay = " << average_delay << endl;
	cout << "Current delay = " << current_delay << endl;

	// TODO: make sure data types are correct here
	double average_rate = average_delay == 0 ? 0 : 1 / average_delay;
	double current_rate = current_delay == 0 ? 0 : 1 / current_delay;

	if(current_rate > average_rate){
		cout << "Sender::state_receive_packet: decreasing sending rate" << endl;
		// decrease the rate

		setRate(0, average_delay);
	}
	else if(current_rate < (average_rate - ATP_THETA * current_rate)){
		cout << "Sender::state_receive_packet: increasing sending rate" << endl;
		// TODO: Figure out what ATP_THETA should be
		// increase the rate

		current_rate = current_rate + ((average_rate - current_rate) / ATP_K);
		setRate(0, 1/current_rate);
	}
	else{
		cout << "Sender::state_receive_packet: rate is staying the same" << endl;
	}
}


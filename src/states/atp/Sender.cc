/*
 * Sender.cc
 *
 *  Created on: Jun 17, 2011
 *      Author: philipbl
 */

#include "../../../headers/states/atp/Sender.h"

//#define PRINT_DEBUG


#ifdef PRINT_DEBUG
	#define DEBUG(str) cout << str << endl;
#else
	#define DEBUG(str)
#endif

Sender::Sender() {
}

Sender::~Sender() {
}

void Sender::state_enter(Context* c){
	ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;

	u_int32_t average_delay = ccc->get_average_delay();

	// Since the SYN + ACK probe was received, just set rate to delay
	// TODO: make sure units are correct here
	setDelay(0, average_delay);
}

void Sender::state_exit(Context* c){

}

void Sender::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e){
	//cout << "Sender::state_receive_packet: entered" << endl;
	ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;
	ATPPacket* packet = ATPPacket::convert_to_atp_packet(e->get_packet());
	assert(packet != 0);

	DEBUG("Sender::state_receive_packet:");
	printPacket(packet);

	// Don't care about the packet if it is not an ACK
	if(!packet->is_tcp_ack() || !between_equal_right(ccc->get_snd_una(), packet->get_tcp_ack_number(), ccc->get_snd_max())){
		DEBUG("Sender::state_receive_packet: didn't receive an ACK or it is out of order");
		return;
	}

	update_context(c, packet);

	update_sending_rate(c, packet);

}

void Sender::state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e){
	//cout << "Sender::state_send_packet: entered" << endl;

	super::state_send_packet(c, q, e);
}

void Sender::state_send_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e) {
	//cout << "Sender::state_send_buffer_not_empty" << endl;
	send_packets(c, q, e);
}

void Sender::update_sending_rate(Context * c, ATPPacket * p){
	ATPCongestionControlContext* ccc = (ATPCongestionControlContext* )c;

	u_int32_t new_delay = p->get_atp_average_delay();
	long int current_delay = getMicroseconds();

	//cout << "\tNew delay = " << new_delay << endl;
	//cout << "\tCurrent delay = " << current_delay << endl;

	double new_rate = 1.0/new_delay;
	double current_rate = 1.0/current_delay;

	//cout << "\tNew rate = " << new_rate << endl;
	//cout << "\tCurrent rate = " << current_rate << endl;
	//cout << "\t" << current_rate << " < " << (new_rate - ATP_THETA * current_rate) << endl;


	if(current_rate > new_rate){
		DEBUG("Sender::state_receive_packet: decreasing sending rate");
		// decrease the rate

		setDelay(0, new_delay);
	}
	else if(current_rate < (new_rate - ATP_THETA * current_rate)){
		DEBUG("Sender::state_receive_packet: increasing sending rate");
		// TODO: Figure out what ATP_THETA should be
		// increase the rate

		current_rate = current_rate + ((new_rate - current_rate) / ATP_K);
		current_delay = 1.0/current_rate;

		//cout << "\tAdjusted rate = " << current_rate << endl;
		//cout << "\tAdjusted delay = " << current_delay << endl;

		setDelay(0, current_delay);
	}
	else{
		DEBUG("Sender::state_receive_packet: rate is staying the same");
	}
}


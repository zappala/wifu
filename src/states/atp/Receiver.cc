/*
 * Receiver.cc
 *
 *  Created on: Jun 17, 2011
 *      Author: philipbl
 */

#include "states/atp/Receiver.h"

//#define PRINT_DEBUG

#ifdef PRINT_DEBUG
	#define DEBUG(str) cout << str << endl;
#else
	#define DEBUG(str)
#endif

Receiver::Receiver() {
}

Receiver::~Receiver() {
}


void Receiver::state_enter(Context* c){
	ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;

	// Since SYN was received, just set max delay to average delay
	ccc->set_average_delay(ccc->get_max_delay());
}

void Receiver::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e){
	ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;

	ATPPacket* packet = ATPPacket::convert_to_atp_packet(e->get_packet());
	assert(packet != 0);

	DEBUG("Receiver::state_receive_packet:");
	printPacket(packet);

	update_context(c, packet);

	DEBUG("Receiver::state_receive_packet: calculating new delay");

	u_int32_t new_max_delay = packet->get_atp_max_delay();
	u_int32_t old_delay = ccc->get_average_delay();

	double new_delay = calculateDelay(new_max_delay, old_delay, ATP_BETA);
	ccc->set_average_delay(new_delay);
}

void Receiver::state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e){
	DEBUG("Receiver::state_send_packet: entered");

	ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;
	ATPPacket* packet = ATPPacket::convert_to_atp_packet(e->get_packet());
	assert(packet != 0);

	if(packet->is_tcp_ack() && packet->get_data_length_bytes() == 0){
		// this is a true ACK, no data

		//cout << "\tReading delay: " << ccc->get_reading_delay() << endl;
		//cout << "\tAverage delay: " << ccc->get_average_delay() << "\n" << endl;

		// TODO: FIX ME

		if(ccc->get_reading_delay() > ccc->get_average_delay()){
		//if(300 > ccc->get_average_delay()){
			DEBUG("Receiver::state_send_packet : reading rate is slower (" << ccc->get_reading_delay() << ")");
			packet->set_atp_average_delay(ccc->get_reading_delay());
			//packet->set_atp_average_delay(300);
		}
		else{
			DEBUG("Receiver::state_send_packet : average rate is slower (" << ccc->get_average_delay() << ")");
			packet->set_atp_average_delay(ccc->get_average_delay());
		}

		send_packet(packet, e->get_socket());
	}
}

void Receiver::state_send_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e) {
	ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;

	DEBUG("Receiver::state_send_buffer_not_empty : switching to Sender state");

	Sender* sender = new Sender();
	ccc->set_state(sender);
	sender->state_send_buffer_not_empty(c, q, e);
}

void Receiver::state_receive_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent* e) {
	ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;

	ccc->start_timer();
}

void Receiver::state_receive_buffer_not_full(Context* c, QueueProcessor<Event*>* q, ReceiveBufferNotFullEvent* e) {
	ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;

	u_int32_t time_diff = ccc->stop_timer();
	ccc->set_reading_delay(time_diff);
}


u_int32_t Receiver::calculateDelay(u_int32_t max, u_int32_t previous, double beta){
	u_int32_t temp = (beta * previous + (1 - beta) * max);

	DEBUG("Receiver::calculateDelay : new delay = " << temp);

	return temp;
}


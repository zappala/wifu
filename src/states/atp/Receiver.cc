/*
 * receiveData.cc
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
void Receiver::state_exit(Context* c){

}

void Receiver::state_receive(Context* c, QueueProcessor<Event*>* q, ReceiveEvent* e){
	DEBUG("Receiver::state_receive : getting new clock difference");
	ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;

	u_int32_t time = ccc->get_time_difference();

	ccc->set_reading_delay(time);
}

void Receiver::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e){
	ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;

	ATPPacket* packet = dynamic_cast<ATPPacket *>(e->get_packet());
	assert(packet != 0);

	DEBUG("Receiver::state_receive_packet:");
	printPacket(packet);

	if(packet->is_tcp_syn()){
		DEBUG("Receiver::state_receive_packet: received a SYN, starting over");

		ccc->set_max_delay(packet->get_atp_max_delay());
		state_enter(ccc);
		return;
	}

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

	if(packet->is_tcp_ack()){

		//cout << "\tReading delay: " << ccc->get_reading_delay() << endl;
		//cout << "\tAverage delay: " << ccc->get_average_delay() << "\n" << endl;

		//if(ccc->get_reading_delay() > ccc->get_average_delay()){
		//	DEBUG("Receiver::state_send_packet : reading rate is slower (" << ccc->get_reading_delay() << ")");
		//	packet->set_atp_average_delay(ccc->get_reading_delay());
		//}
		//else{
		//	DEBUG("Receiver::state_send_packet : average rate is slower (" << ccc->get_average_delay() << ")");
			packet->set_atp_average_delay(ccc->get_average_delay());
		//}

		send_packet(packet, e->get_socket());
	}

}

// TODO: make sure casing is correct!!
u_int32_t Receiver::calculateDelay(u_int32_t max, u_int32_t previous, double beta){
	u_int32_t temp = (beta * previous + (1 - beta) * max);

	DEBUG("Receiver::calculateDelay : new delay = " << temp);

	return temp;
}


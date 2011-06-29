/*
 * receiveData.cc
 *
 *  Created on: Jun 17, 2011
 *      Author: philipbl
 */

#include "../../../headers/states/atp/Receiver.h"

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



void Receiver::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e){
	ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;
	ATPPacket* packet = dynamic_cast<ATPPacket *>(e->get_packet());
	assert(packet != 0);

	cout << "Receiver::state_receive_packet:" << endl;
	printPacket(packet);



	if(packet->is_tcp_syn()){
		cout << "Receiver::state_receive_packet: received a SYN, starting over" << endl;

		ccc->set_max_delay(packet->get_atp_max_delay());
		state_enter(ccc);
		return;
	}

	cout << "Receiver::state_receive_packet: calculating new delay" << endl;

	u_int32_t max_delay = packet->get_atp_max_delay();
	u_int32_t previous_delay = ccc->get_average_delay();

	double new_delay = calculateDelay(max_delay, previous_delay, ATP_BETA);
	ccc->set_average_delay(new_delay);
}

void Receiver::state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e){
	cout << "Receiver::state_send_packet: entered" << endl;

	ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;
	ATPPacket* packet = ATPPacket::convert_to_atp_packet(e->get_packet());
	assert(packet != 0);

	if(packet->is_tcp_ack()){
		cout << "Receiver::state_send_packet: marking packet with delay (" << ccc->get_average_delay() << ")" << endl;

		packet->set_atp_average_delay(ccc->get_average_delay());
		send_packet(packet, e->get_socket());
	}

}

// TODO: make sure casing is correct!!
double Receiver::calculateDelay(u_int32_t max, u_int32_t previous, double beta){
	return (beta * previous + (1 - beta) * max);
}


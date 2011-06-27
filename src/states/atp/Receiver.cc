/*
 * receiveData.cc
 *
 *  Created on: Jun 17, 2011
 *      Author: philipbl
 */

#include "../../../headers/states/atp/Receiver.h"

Receiver::Receiver() {
	// TODO Auto-generated constructor stub

}

Receiver::~Receiver() {
	// TODO Auto-generated destructor stub
}

void Receiver::state_enter(Context* c){
	ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;

	// Since SYN was received, just set max delay to average delay
	ccc->set_average_delay(ccc->get_max_delay());
}
void Receiver::state_exit(Context* c){

}



void Receiver::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e){
	cout << "Receiver::state_receive_packet: entered" << endl;

	ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;
	ATPPacket* packet = (ATPPacket*) e->get_packet();

	cout << "-------PACKET-------" << endl;
	cout << "SYN: " << packet->is_tcp_syn() << endl;
	cout << "ACK: " << packet->is_tcp_ack() << endl;
	cout << "MAX DELAY: " << packet->get_atp_max_delay() << endl;
	cout << "AVG DELAY: " << packet->get_atp_average_delay() << endl;
	cout << "data: " << packet->get_data() << endl;
	cout << "--------------------" << endl;



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
	ATPPacket* packet = (ATPPacket*) e->get_packet();

	if(packet->is_tcp_ack()){
		cout << "Receiver::state_send_packet: marking packet with delay (" << ccc->get_average_delay() << ")" << endl;

		packet->set_atp_average_delay(ccc->get_average_delay());
	}

}

// TODO: make sure casing is correct!!
double Receiver::calculateDelay(u_int32_t max, u_int32_t previous, double beta){
	return (beta * previous + (1 - beta) * max);
}

/*
 * Sender.cc
 *
 *  Created on: Jun 17, 2011
 *      Author: philipbl
 */

#include "../../../headers/states/atp/Sender.h"

Sender::Sender() : SendRateLimiter() {
	// TODO Auto-generated constructor stub

}

Sender::~Sender() {
	// TODO Auto-generated destructor stub
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
	ATPPacket* packet = (ATPPacket*) e->get_packet();

	cout << "-------PACKET-------" << endl;
	cout << "SYN: " << packet->is_tcp_syn() << endl;
	cout << "ACK: " << packet->is_tcp_ack() << endl;
	cout << "MAX DELAY: " << packet->get_atp_max_delay() << endl;
	cout << "AVG DELAY: " << packet->get_atp_average_delay() << endl;
	cout << "data: " << packet->get_data() << endl;
	cout << "--------------------" << endl;


	// Don't care about the packet if it is not an ACK
	// TODO: Maybe I want to know the delay of FIN packets
	if(!packet->is_tcp_ack()){
		cout << "Sender::state_receive_packet: didn't receive an ACK, don't care about it" << endl;
		return;
	}

	// If for some reason a SYN probe gets sent out again, it will get taken care of
	if(packet->is_tcp_syn()){
		cout << "Sender::state_receive_packet: received a SYN + ACK, starting over" << endl;

		ccc->set_average_delay(packet->get_atp_average_delay());
		state_enter(ccc);
		return;
	}

	u_int32_t average_delay = ccc->get_average_delay();
	long int current_delay = getNanoseconds();

	// TODO: make sure data types are correct here
	double average_rate = 1 / average_delay;
	double current_rate = 1 / current_delay;


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
}

void Sender::state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e){
	cout << "Sender::state_send_packet: entered" << endl;

	ATPPacket* packet = (ATPPacket*) e->get_packet();

	cout << "-------PACKET-------" << endl;
	cout << "SYN: " << packet->is_tcp_syn() << endl;
	cout << "ACK: " << packet->is_tcp_ack() << endl;
	cout << "MAX DELAY: " << packet->get_atp_max_delay() << endl;
	cout << "AVG DELAY: " << packet->get_atp_average_delay() << endl;
	cout << "data: " << packet->get_data() << endl;
	cout << "--------------------" << endl;

	super::state_send_packet(c, q, e);
}

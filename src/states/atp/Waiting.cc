/*
 * Waiting.cc
 *
 *  Created on: Jun 17, 2011
 *      Author: philipbl
 */

#include "../../headers/states/atp/Waiting.h"

Waiting::Waiting() {
}

Waiting::~Waiting() {
}

void Waiting::state_enter(Context* c){

}

void Waiting::state_exit(Context* c){

}



void Waiting::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e){
	ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;
	ATPPacket* packet = (ATPPacket*) e->get_packet();

	cout << "-------PACKET-------" << endl;
	cout << "SYN: " << packet->is_tcp_syn() << endl;
	cout << "ACK: " << packet->is_tcp_ack() << endl;
	cout << "MAX DELAY: " << packet->get_atp_max_delay() << endl;
	cout << "AVG DELAY: " << packet->get_atp_average_delay() << endl;
	cout << "data: " << packet->get_data() << endl;
	cout << "--------------------" << endl;


	if(packet->is_tcp_ack() && packet->is_tcp_syn()){
		cout << "Waiting::state_receive_packet: received SYN + ACK packet (creating Sender)" << endl;

		// Starting the sender
		ccc->set_average_delay(packet->get_atp_average_delay());
		ccc->set_state(new Sender());
	}
	else if(packet->is_tcp_syn()){
		cout << "Waiting::state_receive_packet: received SYN packet (creating Receiver)" << endl;

		// Starting the receiver
		ccc->set_max_delay(packet->get_atp_max_delay());
		ccc->set_state(new Receiver());
	}

}


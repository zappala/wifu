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

void Waiting::state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e){
	cout << "Waiting::state_send_packet : sending SYN packet" << endl;

	ATPPacket* packet = ATPPacket::convert_to_atp_packet(e->get_packet());
	assert(packet != 0);

	// sending SYN packet
	send_packet(packet, e->get_socket());
}



void Waiting::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e){
	ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;

	ATPPacket* packet = ATPPacket::convert_to_atp_packet(e->get_packet());
	assert(packet != 0);


	cout << "Waiting::state_receive_packet:" << endl;
	printPacket(packet);

	if(packet->is_tcp_ack() && packet->is_tcp_syn()){
		cout << "Waiting::state_receive_packet: received SYN + ACK packet (creating Sender)" << endl;

		// Starting the sender
		ccc->set_average_delay(packet->get_atp_average_delay());
		ccc->set_receiver_window_size(packet->get_tcp_receive_window_size());

		ccc->set_state(new Sender());
	}
	else if(packet->is_tcp_syn()){
		cout << "Waiting::state_receive_packet: received SYN packet (creating Receiver)" << endl;

		// Starting the receiver
		ccc->set_max_delay(packet->get_atp_max_delay());
		ccc->set_state(new Receiver());
	}

}


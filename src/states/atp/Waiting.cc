/*
 * Waiting.cc
 *
 *  Created on: Jun 17, 2011
 *      Author: philipbl
 */

#include "../../headers/states/atp/Waiting.h"

//#define PRINT_DEBUG

#ifdef PRINT_DEBUG
	#define DEBUG(str) cout << str << endl;
#else
	#define DEBUG(str)
#endif

Waiting::Waiting() {
}

Waiting::~Waiting() {
}

void Waiting::state_enter(Context* c){
}

void Waiting::state_exit(Context* c){
}

void Waiting::state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e){
	DEBUG("Waiting::state_send_packet : sending SYN packet");

	ATPPacket* packet = ATPPacket::convert_to_atp_packet(e->get_packet());
	assert(packet != 0);

	// sending SYN packet
	send_packet(packet, e->get_socket());
}



void Waiting::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e){
	ATPCongestionControlContext* ccc = (ATPCongestionControlContext*) c;

	ATPPacket* packet = ATPPacket::convert_to_atp_packet(e->get_packet());
	assert(packet != 0);


	DEBUG("Waiting::state_receive_packet:");
	printPacket(packet);

	if(packet->is_tcp_ack() && packet->is_tcp_syn()){
		DEBUG("Waiting::state_receive_packet: received SYN + ACK packet (creating Sender)");

		// Starting the sender
		update_context(c, packet);
		ccc->set_average_delay(packet->get_atp_average_delay());
		ccc->set_state(new Sender());
	}
	else if(packet->is_tcp_syn()){
		DEBUG("Waiting::state_receive_packet: received SYN packet (creating Receiver)");

		// Starting the receiver
		ccc->set_max_delay(packet->get_atp_max_delay());
		ccc->set_state(new Receiver());
	}

}


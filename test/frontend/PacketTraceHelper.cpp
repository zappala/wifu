#include "../headers/PacketTraceHelper.h"

void compare_traces(NetworkTrace& expected) {
    PacketLogReader reader(LOG_FILENAME);
    NetworkTrace* actual = reader.get_trace();

    ASSERT_EQ(expected, *actual) << expected.get_packet_trace(*actual);
}

TCPPacket* get_base_tcp_packet(int protocol) {
	TCPPacket* p;
	if(protocol == TCP_ATP){
		p = new ATPPacket();
	}
	else{
		p = new TCPPacket();
	}

    p->set_ip_protocol(protocol);
    string saddr = "127.0.0.1";
    string daddr = saddr;
    p->set_ip_source_address_s(saddr);
    p->set_ip_destination_address_s(daddr);
    return p;
}

TCPPacket* get_base_tcp_packet_ts(int protocol) {
    TCPPacket* p = get_base_tcp_packet(protocol);
    p->insert_tcp_header_option(new TCPTimestampOption());
    p->set_data((unsigned char*)"", 0);
    return p;
}

TCPPacket* get_syn(int protocol) {
    TCPPacket* syn = get_base_tcp_packet_ts(protocol);
    syn->set_destination_port(5002);
    syn->set_source_port(1000);
    syn->set_tcp_sequence_number(1);
    syn->set_tcp_ack_number(0);
    syn->set_tcp_syn(true);
    return syn;
}

TCPPacket* get_synack(int protocol) {
    TCPPacket* synack = get_base_tcp_packet_ts(protocol);
    synack->set_destination_port(1000);
    synack->set_source_port(5002);
    synack->set_tcp_sequence_number(1);
    synack->set_tcp_ack_number(2);
    synack->set_tcp_syn(true);
    synack->set_tcp_ack(true);
    return synack;
}

TCPPacket* get_ack(int protocol) {
    TCPPacket* ack = get_base_tcp_packet_ts(protocol);
    ack->set_destination_port(5002);
    ack->set_source_port(1000);
    ack->set_tcp_sequence_number(2);
    ack->set_tcp_ack_number(2);
    ack->set_tcp_ack(true);
    return ack;
}

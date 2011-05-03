#include "../headers/PacketTraceHelper.h"

void compare_traces(NetworkTrace& expected) {
    PacketLogReader reader(LOG_FILENAME);
    NetworkTrace* actual = reader.get_trace();

    ASSERT_EQ(expected, *actual) << expected.get_packet_trace(*actual);
}

TCPPacket* get_base_tcp_packet() {
    TCPPacket* p = new TCPPacket();
    p->set_ip_protocol(SIMPLE_TCP);
    p->set_ip_source_address_s("127.0.0.1");
    p->set_ip_destination_address_s("127.0.0.1");
    return p;
}

TCPPacket* get_base_tcp_packet_ts() {
    TCPPacket* p = get_base_tcp_packet();
    p->insert_tcp_header_option(new TCPTimestampOption());
    p->set_data((unsigned char*)"", 0);
    return p;
}

TCPPacket* get_syn() {
    TCPPacket* syn = get_base_tcp_packet_ts();
    syn->set_destination_port(5002);
    syn->set_source_port(1000);
    syn->set_tcp_sequence_number(1);
    syn->set_tcp_ack_number(0);
    syn->set_tcp_syn(true);
    return syn;
}

TCPPacket* get_synack() {
    TCPPacket* synack = get_base_tcp_packet_ts();
    synack->set_destination_port(1000);
    synack->set_source_port(5002);
    synack->set_tcp_sequence_number(1);
    synack->set_tcp_ack_number(2);
    synack->set_tcp_syn(true);
    synack->set_tcp_ack(true);
    return synack;
}

TCPPacket* get_ack() {
    TCPPacket* ack = get_base_tcp_packet_ts();
    ack->set_destination_port(5002);
    ack->set_source_port(1000);
    ack->set_tcp_sequence_number(2);
    ack->set_tcp_ack_number(2);
    ack->set_tcp_ack(true);
    return ack;
}

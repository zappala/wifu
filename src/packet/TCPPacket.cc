#include "packet/TCPPacket.h"

TCPPacket::TCPPacket() : WiFuPacket(), data_set_(false) {
    init();
}

//TCPPacket::TCPPacket(IPPacket& p) : WiFuPacket(p) {
//    init();
//}
//
//TCPPacket::TCPPacket(unsigned char* buffer, int length) : WiFuPacket(buffer, length) {
//    init();
//}

TCPPacket::~TCPPacket() {

}

unsigned char* TCPPacket::get_data() {
    return get_next_header() + get_tcp_header_length_bytes();
}

int TCPPacket::get_data_length_bytes() {
    return get_ip_tot_length() - get_ip_header_length_bytes() - get_tcp_header_length_bytes();
}

void TCPPacket::set_data(unsigned char* data, int length) {
    if (data_set_) {
        // Can not set the data twice
        throw IllegalStateException();
    }

    memcpy(get_data(), data, length);
    data_set_ = true;

    u_int8_t tcp_len = get_tcp_header_length_bytes();
    u_int8_t ip_len = get_ip_header_length_bytes();
    set_ip_tot_length(ip_len + tcp_len + length);
}

void TCPPacket::pack() {
    //    cout << "TCPPacket::pack()" << endl;
    GetTCPHeaderOptionsDataVisitor visitor(get_options_pointer());
    options_.accept(&visitor);
    visitor.append_padding();

    // TODO: convert options to network byte order
//    u_int32_t options_size = get_data() - get_options_pointer();
//    assert(!(options_size % sizeof (u_int32_t)));
//
//    for (int i = 0; i < options_size; i += sizeof (u_int32_t)) {
//        u_int32_t* current = (u_int32_t*) (get_options_pointer() + i);
//        *current = htonl(*current);
//    }
}

u_int32_t TCPPacket::get_tcp_sequence_number() const {
    return ntohl(tcp_->seq);
}

void TCPPacket::set_tcp_sequence_number(u_int32_t seq_num) {
    tcp_->seq = htonl(seq_num);
}

u_int32_t TCPPacket::get_tcp_ack_number() const {
    return ntohl(tcp_->ack_seq);
}

void TCPPacket::set_tcp_ack_number(u_int32_t ack_num) {
    tcp_->ack_seq = htonl(ack_num);
}

int TCPPacket::get_tcp_header_length_bytes() const {
    return get_tcp_data_offset() * 4;
}

u_int16_t TCPPacket::get_tcp_data_offset() const {
    return tcp_->doff;
}

void TCPPacket::set_tcp_data_offset(u_int16_t length) {
    tcp_->doff = length;
}

bool TCPPacket::is_tcp_urg() const {
    return tcp_->urg;
}

void TCPPacket::set_tcp_urg(bool urg) {
    tcp_->urg = urg;
}

bool TCPPacket::is_tcp_ack() const {
    return tcp_->ack;
}

void TCPPacket::set_tcp_ack(bool ack) {
    tcp_->ack = ack;
}

bool TCPPacket::is_tcp_psh() const {
    return tcp_->psh;
}

void TCPPacket::set_tcp_psh(bool psh) {
    tcp_->psh = psh;
}

bool TCPPacket::is_tcp_rst() const {
    return tcp_->rst;
}

void TCPPacket::set_tcp_rst(bool rst) {
    tcp_->rst = rst;
}

bool TCPPacket::is_tcp_syn() const {
    return tcp_->syn;
}

void TCPPacket::set_tcp_syn(bool syn) {
    tcp_->syn = syn;
}

bool TCPPacket::is_tcp_fin() const {
    return tcp_->fin;
}

void TCPPacket::set_tcp_fin(bool fin) {
    tcp_->fin = fin;
}

u_int16_t TCPPacket::get_tcp_receive_window_size() const {
    return ntohs(tcp_->window);
}

void TCPPacket::set_tcp_receive_window_size(u_int16_t window) {
    tcp_->window = htons(window);
}

u_int16_t TCPPacket::get_tcp_checksum() const {
    return tcp_->check;
}

void TCPPacket::set_tcp_checksum(u_int16_t checksum) {
    tcp_->check = checksum;
}

u_int16_t TCPPacket::get_tcp_urgent_pointer() const {
    return ntohs(tcp_->urg_ptr);
}

void TCPPacket::set_tcp_urgent_pointer(u_int16_t urg_ptr) {
    tcp_->urg_ptr = htons(urg_ptr);
}

void TCPPacket::calculate_and_set_tcp_checksum() {
    set_tcp_checksum(0);
    set_tcp_checksum(compute_next_checksum());
}

bool TCPPacket::is_valid_tcp_checksum() {
    u_int16_t current_checksum = get_tcp_checksum();
    calculate_and_set_tcp_checksum();
    u_int16_t calculated_checksum = get_tcp_checksum();
    set_tcp_checksum(current_checksum);
    return current_checksum == calculated_checksum;
}

void TCPPacket::init() {
    tcp_ = (struct tcphdr*) get_next_header();
    set_tcp_data_offset(sizeof (struct tcphdr) / 4);
    set_ip_tot_length(get_ip_header_length_bytes() + get_tcp_header_length_bytes());
}

bool TCPPacket::is_naked_ack() {
    if (is_tcp_ack() &&
            get_data_length_bytes() == 0 &&
            !is_tcp_fin() &&
            !is_tcp_psh() &&
            !is_tcp_rst() &&
            !is_tcp_syn() &&
            !is_tcp_urg()) {
        return true;
    }
    return false;
}

int TCPPacket::max_data_length() {
    return IPPacket::max_data_length() - get_tcp_header_length_bytes();
}

string TCPPacket::to_s() const {
    stringstream s;
    s << IPPacket::to_s() << endl
            << "tcp "
            << get_source_port() << " "
            << get_destination_port() << " "
            << get_tcp_sequence_number() << " "
            << get_tcp_ack_number() << " "
            << get_tcp_header_length_bytes() << " "
            << is_tcp_urg() << " "
            << is_tcp_ack() << " "
            << is_tcp_psh() << " "
            << is_tcp_rst() << " "
            << is_tcp_syn() << " "
            << is_tcp_fin() << " "
            << get_tcp_receive_window_size();
    return s.str();
}

string TCPPacket::to_s_format() const {
    stringstream s;
    s << IPPacket::to_s_format() << endl
            << "# tcp sport dport seq_num ack_num header_length URG ACK PSH RST SYN FIN rcv_wnd";
    return s.str();
}

bool TCPPacket::operator ==(const IPPacket& other) const {
    //    cout << "TCPPacket::operator ==()" << endl;
    if (!WiFuPacket::operator ==(other)) {
        return false;
    }

    TCPPacket const* other_ptr = dynamic_cast<TCPPacket const*> (&other);

    bool equal = other_ptr != NULL;
    equal = equal && tcp_->ack == other_ptr->tcp_->ack;
    equal = equal && tcp_->ack_seq == other_ptr->tcp_->ack_seq;
    equal = equal && tcp_->check == other_ptr->tcp_->check;
    equal = equal && tcp_->dest == other_ptr->tcp_->dest;
    equal = equal && tcp_->doff == other_ptr->tcp_->doff;
    equal = equal && tcp_->fin == other_ptr->tcp_->fin;
    equal = equal && tcp_->psh == other_ptr->tcp_->psh;
    equal = equal && tcp_->res1 == other_ptr->tcp_->res1;
    equal = equal && tcp_->res2 == other_ptr->tcp_->res2;
    equal = equal && tcp_->rst == other_ptr->tcp_->rst;
    equal = equal && tcp_->seq == other_ptr->tcp_->seq;
    equal = equal && tcp_->source == other_ptr->tcp_->source;
    equal = equal && tcp_->syn == other_ptr->tcp_->syn;
    equal = equal && tcp_->urg == other_ptr->tcp_->urg;
    equal = equal && tcp_->urg_ptr == other_ptr->tcp_->urg_ptr;
    // TODO: we cannot guarantee what the receive window will be on tests
    //equal = equal && tcp_->window == other_ptr->tcp_->window;

    return equal;
}

bool TCPPacket::operator !=(const IPPacket& other) const {
    //    cout << "TCPPacket::operator !=()" << endl;
    return !(*this == other);
}

void TCPPacket::insert_tcp_header_option(TCPHeaderOption* option) {
    // TODO: should we remove the (same) option if it exists before inserting it?
    options_.insert(option);

    GetTCPHeaderOptionsLengthVisitor visitor;
    options_.accept(&visitor);
    set_tcp_data_offset(get_tcp_data_offset() + visitor.get_padded_length());
}

TCPHeaderOption* TCPPacket::remove_tcp_header_option(u_int8_t kind) {
    TCPHeaderOption* option = options_.remove(kind);

    GetTCPHeaderOptionsLengthVisitor visitor;
    options_.accept(&visitor);
    set_tcp_data_offset(get_tcp_data_offset() + visitor.get_padded_length());

    return option;
}

TCPHeaderOption* TCPPacket::get_option(u_int8_t kind) {
    //TODO: parse options from payload if doff != sizeof(tcphdr) / 4 && options is empty

    if (options_.empty() && get_tcp_data_offset() > sizeof (tcphdr) / 4) {
        // unparsed options
        u_int8_t length = (get_tcp_data_offset() - (sizeof (tcphdr) / 4)) * 4;
        options_.parse(get_options_pointer(), length);
    }

    FindTCPHeaderOptionVisitor finder(kind);
    options_.accept(&finder);
    return finder.get_option();
}

unsigned char* TCPPacket::get_options_pointer() {
    return get_next_header() + sizeof (struct tcphdr);
}
/*
 * ATPPacket.cc
 *
 *  Created on: Jun 10, 2011
 *      Author: philipbl
 */

#include "packet/ATPPacket.h"


ATPPacket::ATPPacket() : super(), data_set_(false) {
	init();
}

ATPPacket::~ATPPacket() {

}

void ATPPacket::init() {
	super::init();

	update_header();
    set_ip_tot_length(get_ip_header_length_bytes() + get_tcp_header_length_bytes() + get_atp_header_length_bytes());

    // set structure to zeros
    atp_->average_delay = 0;
    atp_->max_delay = 0;
}


unsigned char* ATPPacket::get_data(){
	return super::get_data() + get_atp_header_length_bytes();
}


int ATPPacket::get_data_length_bytes(){
	if(!data_set_){
	    set_ip_tot_length(get_ip_header_length_bytes() + get_tcp_header_length_bytes() + get_atp_header_length_bytes());
	}

	return super::get_data_length_bytes() - get_atp_header_length_bytes();
}


void ATPPacket::set_data(unsigned char* data, int length){
    if (data_set_) {
        // Can not set the data twice
        throw IllegalStateException();
    }

    memcpy(get_data(), data, length);
    data_set_ = true;

    u_int8_t atp_len = get_atp_header_length_bytes();
    u_int8_t tcp_len = get_tcp_header_length_bytes();
    u_int8_t ip_len = get_ip_header_length_bytes();
    set_ip_tot_length(ip_len + tcp_len + atp_len + length);

}


void ATPPacket::pack(){
	move_atp_header();
    set_ip_tot_length(get_ip_header_length_bytes() + get_tcp_header_length_bytes() + get_atp_header_length_bytes());

	super::pack();
}


unsigned char* ATPPacket::get_atp_header(){
	update_header();
	return (unsigned char*)atp_;
}


u_int32_t ATPPacket::get_atp_max_delay(){
	update_header();
	return atp_->max_delay;
}


void ATPPacket::set_atp_max_delay(u_int32_t delay){
	update_header();
	atp_->max_delay = delay;
}


u_int32_t ATPPacket::get_atp_average_delay(){
	update_header();
	return atp_->average_delay;
}


void ATPPacket::set_atp_average_delay(u_int32_t delay){
	update_header();
	atp_->average_delay = delay;
}


int ATPPacket::get_atp_header_length_bytes() const{
	return sizeof(struct atp_packet_header);
}

int ATPPacket::max_data_length(){
	return super::max_data_length() - get_atp_header_length_bytes();
}


string ATPPacket::to_s(){
    stringstream s;
    s << super::to_s() << endl
            << "atp "
            << get_atp_max_delay() << " "
            << get_atp_average_delay();
    return s.str();
}


string ATPPacket::to_s_format() const{
    stringstream s;
    s << super::to_s_format() << endl
      << "# atp max_dealay average_delay";
    return s.str();
}


bool ATPPacket::operator ==(const IPPacket& other) const {
	if(!TCPPacket::operator ==(other)) {
		return false;
	}

	ATPPacket const* other_ptr = dynamic_cast<ATPPacket const*> (&other);

	bool equal = other_ptr != NULL;
	equal = equal && atp_->average_delay == other_ptr->atp_->average_delay;
	equal = equal && atp_->max_delay == other_ptr->atp_->max_delay;

	return equal;
}


bool ATPPacket::operator !=(const IPPacket& other) const {
	return !(*this == other);
}


void ATPPacket::insert_tcp_header_option(TCPHeaderOption* option) {
	super::insert_tcp_header_option(option);

	move_atp_header();
    set_ip_tot_length(get_ip_header_length_bytes() + get_tcp_header_length_bytes() + get_atp_header_length_bytes());

}


TCPHeaderOption* ATPPacket::remove_tcp_header_option(u_int8_t kind) {
	TCPHeaderOption* return_value = super::remove_tcp_header_option(kind);

	move_atp_header();
    set_ip_tot_length(get_ip_header_length_bytes() + get_tcp_header_length_bytes() + get_atp_header_length_bytes());


	return return_value;
}



void ATPPacket::update_header(){
	atp_ = (struct atp_packet_header*) (get_next_header() + get_tcp_header_length_bytes());
}


void ATPPacket::move_atp_header(){
	// store the pointer to the data
	atp_packet_header* old_atp_header = atp_;

	// calculate where the new pointer needs to be (because an option was added)
	update_header();

	if(old_atp_header != atp_){
		// copy structure to new pointer
		memcpy(atp_, old_atp_header, get_atp_header_length_bytes());
	}
}

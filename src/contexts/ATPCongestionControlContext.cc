/*
 * ATPCongestionControlContext.cc
 *
 *  Created on: Jun 16, 2011
 *      Author: philipbl
 */

#include "../../headers/contexts/ATPCongestionControlContext.h"

ATPCongestionControlContext::ATPCongestionControlContext(u_int32_t iss) : TCPTahoeCongestionControlContext(iss), average_delay_(0), receive_buffer_size_(0) {
	set_state(new Waiting());
}

ATPCongestionControlContext::~ATPCongestionControlContext() {

}


u_int32_t ATPCongestionControlContext::get_average_delay() const{
	return average_delay_;
}

void ATPCongestionControlContext::set_average_delay(u_int32_t d){
	average_delay_ = d;
}

u_int32_t ATPCongestionControlContext::get_max_delay() const{
	return max_delay_;
}

void ATPCongestionControlContext::set_max_delay(u_int32_t d){
	max_delay_ = d;
}

void ATPCongestionControlContext::set_reading_delay(u_int32_t d){
	read_delay_ = d;
}

u_int32_t ATPCongestionControlContext::get_reading_delay(){
	return read_delay_;
}

void ATPCongestionControlContext::start_timer(){
	clock_gettime(CLOCK_REALTIME, &start_);
}

u_int32_t ATPCongestionControlContext::stop_timer(){
	struct timespec stop;
	clock_gettime(CLOCK_REALTIME, &stop);

	// converting difference to microseconds
	u_int32_t diff = (stop.tv_sec - start_.tv_sec) * 1000000.0 + ((stop.tv_nsec - start_.tv_nsec) / 1000.0);

	return diff;
}

int ATPCongestionControlContext::get_receive_buffer_size(){
	return receive_buffer_size_;
}

void ATPCongestionControlContext::set_receive_buffer_size(int size){
	receive_buffer_size_ = size;
}

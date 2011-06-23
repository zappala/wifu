/*
 * ATPCongestionControlContext.cc
 *
 *  Created on: Jun 16, 2011
 *      Author: philipbl
 */

#include "../../headers/contexts/ATPCongestionControlContext.h"

ATPCongestionControlContext::ATPCongestionControlContext(u_int32_t iss) : OutstandingDataContext(iss), average_delay_(0) {
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

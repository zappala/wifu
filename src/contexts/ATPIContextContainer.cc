/*
 * ATPIContextContainer.cc
 *
 *  Created on: Jun 16, 2011
 *      Author: philipbl
 */

#include "contexts/ATPIContextContainer.h"

ATPIContextContainer::ATPIContextContainer() : TCPTahoeIContextContainer() {
	u_int32_t iss = 1;
	set_congestion_control(new ATPCongestionControlContext(iss));
}

ATPIContextContainer::~ATPIContextContainer() {

}

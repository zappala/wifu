/*
 * ATPIContextContainer.cc
 *
 *  Created on: Jun 16, 2011
 *      Author: philipbl
 */

#include "../../headers/contexts/ATPIContextContainer.h"

ATPIContextContainer::ATPIContextContainer() : TCPTahoeIContextContainer() {
    congestion_control_ = new ATPCongestionControlContext(1);
}

ATPIContextContainer::~ATPIContextContainer() {

}


IContext* ATPIContextContainer::get_congestion_control() {
    return congestion_control_;
}


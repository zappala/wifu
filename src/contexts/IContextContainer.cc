#include "contexts/IContextContainer.h"

IContextContainer::IContextContainer() {
	reliability_ = new ReliabilityContext();
	cc_ = new CongestionControlContext();
	cm_ = new ConnectionManagerContext();
}

IContext* IContextContainer::get_congestion_control() {
	return cc_;
}

IContext* IContextContainer::get_reliability() {
	return reliability_;
}

IContext* IContextContainer::get_connection_manager() {
	return cm_;
}

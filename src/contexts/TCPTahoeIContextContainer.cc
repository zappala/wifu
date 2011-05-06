#include "contexts/TCPTahoeIContextContainer.h"



TCPTahoeIContextContainer::TCPTahoeIContextContainer() {
    connection_manager_ = new ConnectionManagerContext();
    congestion_control_ = new TCPTahoeCongestionControlContext();
    reliability_ = new TCPTahoeReliabilityContext();
}

TCPTahoeIContextContainer::~TCPTahoeIContextContainer() {

}

IContext* TCPTahoeIContextContainer::get_connection_manager() {
    return connection_manager_;
}

IContext* TCPTahoeIContextContainer::get_congestion_control() {
    return congestion_control_;
}

IContext* TCPTahoeIContextContainer::get_reliability() {
    return reliability_;
}

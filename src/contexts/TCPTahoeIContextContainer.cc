#include "contexts/TCPTahoeIContextContainer.h"


TCPTahoeIContextContainer::TCPTahoeIContextContainer() {
    cm_ = new ConnectionManagerContext();
}

TCPTahoeIContextContainer::~TCPTahoeIContextContainer() {

}

IContext* TCPTahoeIContextContainer::get_connection_manager() {
    return cm_;
}

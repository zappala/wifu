#include "PortManagerFactory.h"

PortManagerFactory::PortManagerFactory() {

}

PortManagerFactory& PortManagerFactory::instance() {
    static PortManagerFactory instance_;
    return instance_;
}

void PortManagerFactory::set_creator(IPortManagerCreator* creator) {
    creator_ = creator;
}

IPortManager& PortManagerFactory::create() {
    return creator_->create();
}

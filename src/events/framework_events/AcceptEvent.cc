#include "events/framework_events/AcceptEvent.h"

AcceptEvent::AcceptEvent() : AddressEvent() {

}

AcceptEvent::~AcceptEvent() {

}

void AcceptEvent::execute(IModule* m) {
    m->imodule_library_accept(this);
}

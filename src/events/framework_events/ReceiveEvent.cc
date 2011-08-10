#include "events/framework_events/ReceiveEvent.h"

ReceiveEvent::ReceiveEvent(gcstring_map& m, gcstring& file, Socket* s) : LibraryEvent(m, file, s) {

}

ReceiveEvent::ReceiveEvent() : LibraryEvent() {
    
}

ReceiveEvent::~ReceiveEvent() {
    
}

void ReceiveEvent::execute(IModule* m) {
    m->imodule_library_receive(this);
}

int ReceiveEvent::get_receive_buffer_size() {
    return atoi(get_map()[N_STRING].c_str());
}
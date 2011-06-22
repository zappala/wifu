#include "events/framework_events/ReceiveEvent.h"

ReceiveEvent::ReceiveEvent(string& message, string& file, Socket* s) : LibraryEvent(message, file, s) {

}

void ReceiveEvent::execute(IModule* m) {
    m->imodule_library_receive(this);
}

int ReceiveEvent::get_receive_buffer_size() {
    return atoi(get_map()[N_STRING].c_str());
}
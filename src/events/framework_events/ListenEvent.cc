#include "events/framework_events/ListenEvent.h"

ListenEvent::ListenEvent() : LibraryEvent() {
    
}

ListenEvent::~ListenEvent() {
}

void ListenEvent::execute(IModule* m) {
    m->imodule_library_listen(this);
}

int ListenEvent::get_back_log() {
    return ((struct ListenMessage*) get_buffer())->n;
}

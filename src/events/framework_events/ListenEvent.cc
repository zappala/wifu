#include "events/framework_events/ListenEvent.h"

ListenEvent::ListenEvent(gcstring_map& m, gcstring& file, Socket* s) : LibraryEvent(m, file, s) {
}

ListenEvent::ListenEvent() : LibraryEvent() {
    
}

ListenEvent::~ListenEvent() {
}

void ListenEvent::execute(IModule* m) {
    m->imodule_library_listen(this);
}

int ListenEvent::get_back_log() {
    return atoi(get_map()[N_STRING].c_str());
}

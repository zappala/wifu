#include "events/framework_events/ListenEvent.h"

ListenEvent::ListenEvent(gcstring& message, gcstring& file, Socket* s) : LibraryEvent(message, file, s) {
}

ListenEvent::~ListenEvent() {
}

void ListenEvent::execute(IModule* m) {
    m->imodule_library_listen(this);
}

int ListenEvent::get_back_log() {
    return atoi(get_map()[N_STRING].c_str());
}

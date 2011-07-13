#include "events/framework_events/AcceptEvent.h"

AcceptEvent::AcceptEvent(gcstring_map& m, gcstring& file, Socket* s) : LibraryEvent(m, file, s) {

}

AcceptEvent::~AcceptEvent() {

}

void AcceptEvent::execute(IModule* m) {
    m->imodule_library_accept(this);
}

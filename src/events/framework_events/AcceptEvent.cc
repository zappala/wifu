#include "events/framework_events/AcceptEvent.h"

AcceptEvent::AcceptEvent(gcstring& message, gcstring& file, Socket* s) : LibraryEvent(message, file, s) {

}

AcceptEvent::~AcceptEvent() {

}

void AcceptEvent::execute(IModule* m) {
    m->imodule_library_accept(this);
}

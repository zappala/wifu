#include "events/framework_events/BindEvent.h"

BindEvent::BindEvent(gcstring& message, gcstring& file, Socket* s) : LibraryEvent(message, file, s) {

}

BindEvent::~BindEvent() {

}

void BindEvent::execute(IModule* m) {
    m->imodule_library_bind(this);
}

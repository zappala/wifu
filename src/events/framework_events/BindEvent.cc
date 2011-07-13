#include "events/framework_events/BindEvent.h"

BindEvent::BindEvent(gcstring_map& m, gcstring& file, Socket* s) : LibraryEvent(m, file, s) {

}

BindEvent::~BindEvent() {

}

void BindEvent::execute(IModule* m) {
    m->imodule_library_bind(this);
}

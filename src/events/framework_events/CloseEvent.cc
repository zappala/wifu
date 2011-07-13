#include "events/framework_events/CloseEvent.h"

CloseEvent::CloseEvent(gcstring_map& m, gcstring& file, Socket* s) : LibraryEvent(m, file, s) {

}

CloseEvent::~CloseEvent() {

}

void CloseEvent::execute(IModule* m) {
    m->imodule_library_close(this);
}

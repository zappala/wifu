#include "events/framework_events/CloseEvent.h"

CloseEvent::CloseEvent(string& message, string& file, Socket* s) : LibraryEvent(message, file, s) {

}

CloseEvent::~CloseEvent() {

}

void CloseEvent::execute(IModule* m) {
    m->imodule_library_close(this);
}

#include "events/framework_events/CloseEvent.h"

CloseEvent::CloseEvent(gcstring& message, gcstring& file, Socket* s) : LibraryEvent(message, file, s) {

}

CloseEvent::~CloseEvent() {

}

void CloseEvent::execute(IModule* m) {
    m->imodule_library_close(this);
}

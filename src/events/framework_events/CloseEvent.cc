#include "events/framework_events/CloseEvent.h"

CloseEvent::CloseEvent() : LibraryEvent() {
    
}

CloseEvent::~CloseEvent() {

}

void CloseEvent::execute(IModule* m) {
    m->imodule_library_close(this);
}

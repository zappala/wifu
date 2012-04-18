#include "events/framework_events/FrameworkEvent.h"

FrameworkEvent::FrameworkEvent(Socket* s) : Event(s, LOW) {

}

FrameworkEvent::FrameworkEvent() : Event() {

}

FrameworkEvent::~FrameworkEvent() {
    
}

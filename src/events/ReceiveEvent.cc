#include "events/ReceiveEvent.h"

ReceiveEvent::ReceiveEvent(string& message, string& file, Socket* s) : LibraryEvent(message, file, s) {

}

void ReceiveEvent::execute(IModule* m) {
    m->library_receive(this);
}
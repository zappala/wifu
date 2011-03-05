#include "events/SendEvent.h"

SendEvent::SendEvent(string& message, string& file, Socket* s) : LibraryEvent(message, file, s) {

}

void SendEvent::execute(IModule* m) {
    m->library_send(this);
}
#include "events/protocol_events/DeleteSocketEvent.h"

DeleteSocketEvent::DeleteSocketEvent(Socket* s) : Event(s) {

}

DeleteSocketEvent::~DeleteSocketEvent() {

}

void DeleteSocketEvent::execute(IModule* m) {
    m->imodule_delete_socket(this);
}

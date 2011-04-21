#include "events/DeleteSocketEvent.h"

DeleteSocketEvent::DeleteSocketEvent(Socket* s) : Event(s) {

}

DeleteSocketEvent::~DeleteSocketEvent() {

}

void DeleteSocketEvent::execute(IModule* m) {
    m->delete_socket(this);
}

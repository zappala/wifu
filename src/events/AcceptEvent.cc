#include "events/AcceptEvent.h"

AcceptEvent::AcceptEvent(string& message, string& file, Socket* s) : LibraryEvent(message, file, s) {}

AcceptEvent::~AcceptEvent() {}

void AcceptEvent::execute(IModule* m) {
	m->library_accept(this);
}

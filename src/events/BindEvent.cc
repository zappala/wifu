#include "events/BindEvent.h"

BindEvent::BindEvent(string& message, string& file, Socket* s) : LibraryEvent(message, file, s) {}

BindEvent::~BindEvent() {}

void BindEvent::execute(IModule* m) {
	m->library_bind(this);
}

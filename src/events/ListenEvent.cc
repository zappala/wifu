#include "events/ListenEvent.h"

ListenEvent::ListenEvent(string& message, string& file, Socket* s) : LibraryEvent(message, file, s) {}

ListenEvent::~ListenEvent() {}

void ListenEvent::execute(IModule* m) {
	m->library_listen(this);
}

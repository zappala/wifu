#include "NullEvent.h"

NullEvent::NullEvent(Socket* s) : Event(s) {}

void NullEvent::execute(IModule* m) {
	// do nothing
}

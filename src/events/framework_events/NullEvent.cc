#include "events/framework_events/NullEvent.h"

NullEvent::NullEvent(Socket* s) : FrameworkEvent(s) {}

void NullEvent::execute(IModule* m) {
	// do nothing
}

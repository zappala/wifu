#include "events/CancelTimerEvent.h"

CancelTimerEvent::CancelTimerEvent(TimeoutEvent* event) : TimerEvent(event) {}

CancelTimerEvent::~CancelTimerEvent() {}

void CancelTimerEvent::execute(IModule* m) {
	m->imodule_cancel_timer(this);
}

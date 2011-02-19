#include "CancelTimerEvent.h"

CancelTimerEvent::CancelTimerEvent(TimeoutEvent* event) : TimerEvent(event) {}

CancelTimerEvent::~CancelTimerEvent() {}

void CancelTimerEvent::execute(IModule* m) {
	m->cancel_timer(this);
}

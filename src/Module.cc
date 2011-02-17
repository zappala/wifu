#include "Module.h"

Module::Module() : IModule(), QueueProcessor<Event*>(), TimeoutHelper() {
	start_processing();
}

Module::Module(IQueue<Event*>* queue) : IModule(), QueueProcessor<Event*>(queue), TimeoutHelper() {
	start_processing();
}

Module::~Module() {

}

void Module::process(Event* e) {
	e->execute(this);
}

void Module::timer_fired(Event* e) {
	TimerFiredEvent* event = (TimerFiredEvent*) e;

	if(is_my_timeout(event)) {
		my_timer_fired(event);
	}
}

void Module::dispatch(Event* e) {
	Dispatcher::instance().enqueue(e);
}

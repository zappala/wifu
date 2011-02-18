#include "Module.h"

Module::Module() : IModule(), QueueProcessor<Event*>() {
	start_processing();
}

Module::Module(IQueue<Event*>* queue) : IModule(), QueueProcessor<Event*>(queue) {
	start_processing();
}

Module::~Module() {

}

void Module::process(Event* e) {
	e->execute(this);
}

void Module::dispatch(Event* e) {
	Dispatcher::instance().enqueue(e);
}

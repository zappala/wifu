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
//    static int i;
//    cout << "Module::process(): " << ++i << " Processing on module: " << type_name(*this) << " Event: " << type_name(*e) << " Socket: " << e->get_socket() << endl;
//    cout.flush();
    e->execute(this);
}

void Module::dispatch(Event* e) {
    Dispatcher::instance().enqueue(e);
}

#include "Dispatcher.h"

Dispatcher& Dispatcher::instance() {
    static Dispatcher instance_;
    return instance_;
}

Dispatcher::~Dispatcher() {
	reset();
}

void Dispatcher::map_event(event_name name, QueueProcessor<Event*>* q) {
    mutex_.wait();
    if (map_[name] == NULL) {
        map_[name] = new vector<QueueProcessor<Event*>*>;
    }

    vector<QueueProcessor<Event*>*>::iterator itr = find(map_[name]->begin(), map_[name]->end(), q);
    if (itr == map_[name]->end()) {
        map_[name]->push_back(q);
    }
    mutex_.post();
}

void Dispatcher::reset() {
    mutex_.wait();
    tr1::unordered_map<event_name, vector<QueueProcessor<Event*>*>*>::iterator itr = map_.begin();
    while (itr != map_.end()) {
        vector<QueueProcessor<Event*>*>* v = itr->second;
        delete v;
        ++itr;
    }
    map_.clear();
    mutex_.post();
}

void Dispatcher::process(Event* e) {
    mutex_.wait();
    //cout << "Event name: " << type_name(*e) << endl;

    //assuming should_enqueue is called in QueueProcessor::enqueue(..) and did its job, this variable should never be NULL
    vector<QueueProcessor<Event*>*>* queue_processors = map_[type_name(*e)];

	for (int i = 0; i < queue_processors->size(); i++) {
		//cout << "Processing: " << type_name(*e) << endl;
		queue_processors->at(i)->enqueue(e);
	}

    mutex_.post();
}

bool Dispatcher::should_enqueue(Event* event) {
	mutex_.wait();
	vector<QueueProcessor<Event*>*>* queue_processors = map_[type_name(*event)];
	mutex_.post();

	if (queue_processors == NULL)
	{
//		log_WARNING(" is not mapped to a QueueProcessor");
		return false;
	}
	else
		return true;
}

Dispatcher::Dispatcher() : QueueProcessor<Event*>() {
    mutex_.init(1);
}

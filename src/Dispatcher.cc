#include "Dispatcher.h"

Dispatcher& Dispatcher::instance() {
	static Dispatcher instance_;
	return instance_;
}

Dispatcher::~Dispatcher() {
	clear();
}

void Dispatcher::map_event(event_name name, QueueProcessor<Event*>* q) {
	if (map_[name] == NULL) {
		map_[name] = new vector<QueueProcessor<Event*>*>;
	}

	vector<QueueProcessor<Event*>*>::iterator itr = find(map_[name]->begin(), map_[name]->end(), q);
	if(itr != map_[name]->end()) {
		return;
	}

	map_[name]->push_back(q);
}

void Dispatcher::reset() {
	clear();
}

void Dispatcher::process(Event * e) {
	cout << "Event name: " << type_name(*e) << endl;
	vector<QueueProcessor<Event*>*>* queue_processors = map_[type_name(*e)];

	if (queue_processors == NULL) {
		return;
	}

	for (int i = 0; i < queue_processors->size(); i++) {
		//cout << "Processing: " << type_name(*e) << endl;
		queue_processors->at(i)->enqueue(e);
	}
}

Dispatcher::Dispatcher() : QueueProcessor<Event*>() {}

void Dispatcher::clear() {
	tr1::unordered_map<event_name, vector<QueueProcessor<Event*>*>*>::iterator itr = map_.begin();
	for (; itr != map_.end(); ++itr) {
		vector<QueueProcessor<Event*>*>* v = itr->second;
		delete v;
	}
}

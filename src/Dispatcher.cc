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
    if (map_.find(name) == map_.end()) {
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
//    cout << "Event name: " << type_name(*e) << endl;

    itr_ = map_.find(type_name(*e));

    if (itr_ != map_.end()) {

        vector<QueueProcessor<Event*>*>* queue_processors = itr_->second;

        for (int i = 0; i < queue_processors->size(); i++) {
//            cout << "Processing: " << type_name(*e) << endl;
            queue_processors->at(i)->enqueue(e);
        }
    }
    mutex_.post();
}


Dispatcher::Dispatcher() : QueueProcessor<Event*>(new PriorityQueue<Event*, PriorityEventComparator>()) {
    mutex_.init(1);
}

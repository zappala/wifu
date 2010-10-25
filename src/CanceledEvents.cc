#include "CanceledEvents.h"
#include "Identifiable.h"

//private

CanceledEvents::CanceledEvents() {
    // Set sem to 1
    sem_.post();
}

CanceledEvents::CanceledEvents(CanceledEvents const& other) {

}

CanceledEvents& CanceledEvents::operator=(CanceledEvents const& other) {

}

//public

CanceledEvents::~CanceledEvents() {

}

CanceledEvents & CanceledEvents::instance() {
    static CanceledEvents instance_;
    return instance_;
}

void CanceledEvents::add(Event * event) {
    sem_.wait();
    ids_.insert(event->get_id());
    sem_.post();
}

void CanceledEvents::remove(Event * event) {
    sem_.wait();
    ids_.erase(event->get_id());
    sem_.post();
}

bool CanceledEvents::is_canceled(Event * event) {
    sem_.wait();
    bool val = ids_.find(event->get_id()) != ids_.end();
    sem_.post();
    return val;
}

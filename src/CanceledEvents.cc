#include "events/CanceledEvents.h"

//private

CanceledEvents::CanceledEvents() : HashSet<Event*>() {

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

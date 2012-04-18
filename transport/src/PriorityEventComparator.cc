#include "PriorityEventComparator.h"


bool PriorityEventComparator::operator()(Event * const& t1, Event * const& t2) {
    return t1->get_priority() < t2->get_priority();
}

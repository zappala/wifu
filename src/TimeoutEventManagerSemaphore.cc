#include "TimeoutEventManagerSemaphore.h"

// private

TimeoutEventManagerSemaphore::TimeoutEventManagerSemaphore() : BinarySemaphore() {
    init(0);

}

// public

TimeoutEventManagerSemaphore::~TimeoutEventManagerSemaphore() {

}

TimeoutEventManagerSemaphore & TimeoutEventManagerSemaphore::instance() {
    static TimeoutEventManagerSemaphore instance_;
    return instance_;
}

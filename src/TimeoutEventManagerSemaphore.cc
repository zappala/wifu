#include "TimeoutEventManagerSemaphore.h"

// private

TimeoutEventManagerSemaphore::TimeoutEventManagerSemaphore() : Semaphore() {

}

// public

TimeoutEventManagerSemaphore::~TimeoutEventManagerSemaphore() {

}

TimeoutEventManagerSemaphore & TimeoutEventManagerSemaphore::instance() {
    static TimeoutEventManagerSemaphore instance_;
    return instance_;
}

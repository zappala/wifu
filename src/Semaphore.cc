#include "Semaphore.h"

Semaphore::Semaphore() {

}

Semaphore::~Semaphore() {

}

void Semaphore::init(int value) {
    sem_init(&sem_, 0, value);
}

void Semaphore::post() {
    sem_post(&sem_);
}

void Semaphore::wait() {
    sem_wait(&sem_);
}

void Semaphore::timed_wait(timespec* ts) {
    sem_timedwait(&sem_, ts);
}

int Semaphore::get_value() {
    int value;
    sem_getvalue(&sem_, & value);
    return value;
}

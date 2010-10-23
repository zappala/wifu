#include "Semaphore.h"

Semaphore::Semaphore(int value) {
    sem_init(&sem_, 0, value);
}

Semaphore::Semaphore() {
    sem_init(&sem_, 0, 0);
}

Semaphore::~Semaphore() {

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

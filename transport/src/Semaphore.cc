

#include "Semaphore.h"

Semaphore::Semaphore() {

}

Semaphore::~Semaphore() {
    sem_destroy(&sem_);
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

void Semaphore::try_wait(void) {
    sem_trywait(&sem_);
}

bool Semaphore::timed_wait(timespec* ts) {
    sem_timedwait(&sem_, ts);

    bool failed = errno == ETIMEDOUT;
    if (failed) {
        errno = 0;
    }
    return failed;
}

int Semaphore::get_value() {
    int value;
    sem_getvalue(&sem_, & value);
    return value;
}

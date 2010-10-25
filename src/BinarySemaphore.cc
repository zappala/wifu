#include "BinarySemaphore.h"

BinarySemaphore::BinarySemaphore() : Semaphore() {

}

BinarySemaphore::~BinarySemaphore() {

}

void BinarySemaphore::init(int value) {
    int i = 0;
    if(value) {
        i = 1;
    }
    this->Semaphore::init(value);
}

void BinarySemaphore::post() {
    sem_trywait(&sem_);
    sem_post(&sem_);
}

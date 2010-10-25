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
    try_wait();
    this->Semaphore::post();
}

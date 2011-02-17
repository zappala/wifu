#include "MainSemaphore.h"

MainSemaphore::~MainSemaphore() {}

MainSemaphore& MainSemaphore::instance() {
	static MainSemaphore instance_;
	return instance_;
}

MainSemaphore::MainSemaphore() : Semaphore() {}

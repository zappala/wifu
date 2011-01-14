#include "observer/Observable.h"

Observable::Observable() {

}

Observable::~Observable() {

}

void Observable::add_observer(Observer* o) {
    observers_.push_back(o);
}

void Observable::notify() {
    for(int i = 0; i < observers_.size(); ++i) {
        observers_[i]->update(this);
    }
}
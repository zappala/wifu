#include "observer/Observable.h"

Observable::Observable() {

}

Observable::~Observable() {

}

void Observable::add_observer(Observer* o) {
    observers_.insert(o);
}

void Observable::notify() {
    itr_ = observers_.begin();
    for(; itr_ != observers_.end(); ++itr_) {
        itr_->update(this);
    }
}
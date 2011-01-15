#include "../headers/contexts/Context.h"

Context::Context() : IContext(), current_(0) {

}

Context::~Context() {

}

void Context::set_state(State* s) {

    if (current_) {
        current_->exit(this);
    }

    delete current_;
    current_ = s;

    if (current_) {
        current_->enter(this);
    }

}

State* Context::get_state() {
    return current_;
}

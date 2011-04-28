#include "packet/TCPHeaderOptionCollection.h"

TCPHeaderOptionCollection::TCPHeaderOptionCollection() : Visitable() {

}

TCPHeaderOptionCollection::~TCPHeaderOptionCollection() {

}

void TCPHeaderOptionCollection::insert(TCPHeaderOption* option) {
    options_.push_back(option);
}

TCPHeaderOption* TCPHeaderOptionCollection::remove(u_int8_t kind) {
    TCPHeaderOption* ret_val = 0;
    int i = 0;

    for (; i < options_.size(); ++i) {
        if (options_[i]->get_kind() == kind) {
            ret_val = options_[i];
            break;
        }
    }

    if (ret_val) {
        options_.erase(options_.begin() + i);
    }

    return ret_val;
}

bool TCPHeaderOptionCollection::contains(u_int8_t kind) {
    for (int i = 0; i < options_.size(); ++i) {
        if (options_[i]->get_kind() == kind) {
            return true;
        }
    }
    return false;
}

u_int8_t TCPHeaderOptionCollection::get_padded_length() {
    // Iterate over the options and return the number of words all options will take up
}

unsigned char* TCPHeaderOptionCollection::get_data() {
    // return a buffer containing all options with any necessary padding
}

void TCPHeaderOptionCollection::accept(Visitor* v) {
    for (int i = 0; i < options_.size(); ++i) {
        v->visit(options_[i]);
        if(v->stop()) {
            break;
        }
    }
}

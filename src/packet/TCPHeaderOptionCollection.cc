#include "packet/TCPHeaderOptionCollection.h"
#include "visitors/RemoveTCPHeaderOptionVisitor.h"
#include "visitors/GetTCPHeaderOptionsLengthVisitor.h"

TCPHeaderOptionCollection::TCPHeaderOptionCollection() : Visitable() {

}

TCPHeaderOptionCollection::~TCPHeaderOptionCollection() {

}

void TCPHeaderOptionCollection::insert(TCPHeaderOption* option) {
    options_.push_back(option);
}

TCPHeaderOption* TCPHeaderOptionCollection::remove(u_int8_t kind) {
    FindTCPHeaderOptionVisitor visitor(kind);
    accept(&visitor);
    TCPHeaderOption* ret_val = visitor.get_option();
    if (ret_val) {
        options_.remove(ret_val);
    }
    return ret_val;
}

bool TCPHeaderOptionCollection::contains(u_int8_t kind) {
    FindTCPHeaderOptionVisitor visitor(kind);
    accept(&visitor);
    TCPHeaderOption* ret_val = visitor.get_option();
    return ret_val != 0;
}

u_int8_t TCPHeaderOptionCollection::get_padded_length() {
    // Iterate over the options and return the number of 32-bid words all options will take up
    GetTCPHeaderOptionsLengthVisitor v;
    accept(&v);
    return v.get_padded_length();
}

unsigned char* TCPHeaderOptionCollection::get_data() {
    // return a buffer containing all options with any necessary padding
}

void TCPHeaderOptionCollection::accept(Visitor* v) {
    list<TCPHeaderOption*>::iterator itr = options_.begin();
    for (; itr != options_.end(); ++itr) {
        v->visit(*itr);
        if(v->stop()) {
            break;
        }
    }
}

#include "packet/TCPHeaderOptionCollection.h"


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

bool TCPHeaderOptionCollection::empty() {
    return options_.empty();
}

void TCPHeaderOptionCollection::parse(unsigned char* options, u_int8_t options_length) {
    if(options_length <= 0) {
        return;
    }

    // end of options
    if(*options == 0) {
        return;
    }

    // No operation
    if(*options == 1) {
        parse(options + 1, options_length - 1);
        return;
    }

    // Create new option
    u_int8_t kind = *options;
    // TODO: ensure we support this kind of option?
    // We could do something similar to the packet factory and register options and create the correct kind
    u_int8_t length = *(options + 1);
    TCPHeaderOption* option = new TCPHeaderOption(kind, length);    
    option->set_data(options, length);
    insert(option);

    parse(options + length, options_length - length);   
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

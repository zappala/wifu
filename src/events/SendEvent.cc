#include "events/SendEvent.h"

SendEvent::SendEvent(string& message, string& file, Socket* s) : LibraryEvent(message, file, s), data_(0), data_length_(-1) {

}

void SendEvent::execute(IModule* m) {
    m->library_send(this);
}

unsigned char * SendEvent::get_data() {
    if (data_ == 0) {
        data_ = (unsigned char*) get_map().at(BUFFER_STRING).c_str();
    }
    return data_;
}

ssize_t SendEvent::data_length() {
    if(data_length_ < 0) {
        data_length_ = atoi(get_map().at(N_STRING).c_str());
    }

    // TODO: move this test to testing
    int string_size = get_map().at(BUFFER_STRING).size();
    assert(string_size == data_length_);
    
    return data_length_;
}
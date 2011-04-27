#include "packet/TCPHeaderOption.h"

TCPHeaderOption::TCPHeaderOption(u_int8_t kind, u_int8_t length) {
    memcpy(get_kind_pointer(), &kind, sizeof(u_int8_t));
    memcpy(get_length_pointer(), &length, sizeof(u_int8_t));
}

TCPHeaderOption::~TCPHeaderOption() {

}

u_int8_t* TCPHeaderOption::get_kind_pointer() {
    return (u_int8_t*) data_;
}

u_int8_t* TCPHeaderOption::get_length_pointer() {
    return (u_int8_t*) (data_ + 1);
}

unsigned char* TCPHeaderOption::get_value_pointer() {
    return data_ + 2;
}

unsigned char* TCPHeaderOption::get_data() {
    return data_;
}

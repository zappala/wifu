#include "packet/TCPHeaderOption.h"

TCPHeaderOption::TCPHeaderOption(u_int8_t kind, u_int8_t length) {
    set_kind(kind);
    set_length(length);
}

TCPHeaderOption::~TCPHeaderOption() {

}

u_int8_t TCPHeaderOption::get_kind() {
    return *data_;
}

void TCPHeaderOption::set_kind(u_int8_t kind) {
    *data_ = kind;
}

u_int8_t TCPHeaderOption::get_length() {
    return *(data_ + 1);
}

void TCPHeaderOption::set_length(u_int8_t length) {
    *(data_ + 1) = length;
}

unsigned char* TCPHeaderOption::get_value() {
    return data_ + 2;
}

unsigned char* TCPHeaderOption::get_data() {
    return data_;
}

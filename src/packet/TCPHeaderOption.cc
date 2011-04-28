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
    assert(length <= TCP_HEADER_OPTION_MAX_SIZE);
    *(data_ + 1) = length;
}

unsigned char* TCPHeaderOption::get_value() {
    return data_ + 2;
}

void TCPHeaderOption::set_value(unsigned char* value, u_int8_t value_length) {
    assert(2 + value_length <= TCP_HEADER_OPTION_MAX_SIZE);
    memcpy(get_value(), value, value_length);
}

unsigned char* TCPHeaderOption::get_data() {
    return data_;
}

void TCPHeaderOption::set_data(unsigned char* data, u_int8_t data_length) {
    assert(data_length <= TCP_HEADER_OPTION_MAX_SIZE);
    memcpy(get_data(), data, data_length);
}

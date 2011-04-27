#include "packet/TCPTimestampOption.h"

TCPTimestampOption::TCPTimestampOption() : TCPHeaderOption(8, 10) {
    timestamp_ = (struct timestamp*) get_value_pointer();
}

TCPTimestampOption::~TCPTimestampOption() {

}

u_int32_t* TCPTimestampOption::get_timestamp_pointer() {
    return &timestamp_->timestamp_value_;
}

u_int32_t* TCPTimestampOption::get_echo_pointer() {
    return &timestamp_->timestamp_echo_reply_;
}

struct timestamp* TCPTimestampOption::get_timestamp_struct_pointer() {
    return timestamp_;
}

void TCPTimestampOption::set_echo_value(u_int32_t echo) {
    memcpy(get_echo_pointer(), &echo, sizeof(echo));
}

void TCPTimestampOption::set_timestamp(u_int32_t timestamp) {
    memcpy(get_timestamp_pointer(), &timestamp, sizeof(timestamp));
}

u_int32_t TCPTimestampOption::get_current_time() {
    return 0;
}
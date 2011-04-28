#include "packet/TCPTimestampOption.h"

TCPTimestampOption::TCPTimestampOption() : TCPHeaderOption(8, 10) {
    timestamp_ = (struct timestamp*) get_value();
}

TCPTimestampOption::~TCPTimestampOption() {

}

u_int32_t TCPTimestampOption::get_timestamp() {
    return timestamp_->timestamp_value_;
}

u_int32_t TCPTimestampOption::get_echo_reply() {
    return timestamp_->timestamp_echo_reply_;
}

struct timestamp* TCPTimestampOption::get_timestamp_struct_pointer() {
    return timestamp_;
}

void TCPTimestampOption::set_echo_reply(u_int32_t echo) {
    timestamp_->timestamp_echo_reply_ = echo;
}

void TCPTimestampOption::set_timestamp(u_int32_t timestamp) {
    timestamp_->timestamp_value_ = timestamp;
}
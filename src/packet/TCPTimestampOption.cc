#include "packet/TCPTimestampOption.h"

TCPTimestampOption::TCPTimestampOption() : TCPHeaderOption(8, 10) {
    
}

TCPTimestampOption::~TCPTimestampOption() {

}

u_int32_t TCPTimestampOption::get_timestamp() {
    return get_wifu_tcp_timestamp()->timestamp_value_;
}

u_int32_t TCPTimestampOption::get_echo_reply() {
    return get_wifu_tcp_timestamp()->timestamp_echo_reply_;
}

struct wifu_tcp_timestamp* TCPTimestampOption::get_wifu_tcp_timestamp() {
    return (struct wifu_tcp_timestamp*) get_value();
}

void TCPTimestampOption::set_echo_reply(u_int32_t echo) {
    get_wifu_tcp_timestamp()->timestamp_echo_reply_ = echo;
}

void TCPTimestampOption::set_timestamp(u_int32_t timestamp) {
    get_wifu_tcp_timestamp()->timestamp_value_ = timestamp;
}
#include "packet/TCPTimestampOption.h"

TCPTimestampOption::TCPTimestampOption() : TCPHeaderOption(TCPOPT_TIMESTAMP, TCPOLEN_TIMESTAMP) {
    
}

TCPTimestampOption::~TCPTimestampOption() {

}

u_int32_t TCPTimestampOption::get_timestamp() {
    return ntohl(get_wifu_tcp_timestamp()->timestamp_value_);
}

u_int32_t TCPTimestampOption::get_echo_reply() {
    return ntohl(get_wifu_tcp_timestamp()->timestamp_echo_reply_);
}

struct wifu_tcp_timestamp* TCPTimestampOption::get_wifu_tcp_timestamp() {
    return (struct wifu_tcp_timestamp*) get_value();
}

void TCPTimestampOption::set_echo_reply(u_int32_t echo) {
    get_wifu_tcp_timestamp()->timestamp_echo_reply_ = htonl(echo);
}

void TCPTimestampOption::set_timestamp(u_int32_t timestamp) {
    get_wifu_tcp_timestamp()->timestamp_value_ = htonl(timestamp);
}

gcstring TCPTimestampOption::to_s() {
    char buffer[100];
    sprintf(buffer, "Timestamp: %ud Echo-reply: %ud", get_timestamp(), get_echo_reply());
    return buffer;
}
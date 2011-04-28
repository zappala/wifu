#include "visitors/GetTCPHeaderOptionsDataVisitor.h"

GetTCPHeaderOptionsDataVisitor::GetTCPHeaderOptionsDataVisitor(unsigned char* options) : options_(options) {

}

void GetTCPHeaderOptionsDataVisitor::visit(TCPHeaderOption* option) {
    GetTCPHeaderOptionsLengthVisitor::visit(option);

    memcpy(options_, option->get_data(), option->get_length());
    options_ += option->get_length();
}

void GetTCPHeaderOptionsDataVisitor::append_padding() {
    memset(options_, 0, get_num_padded_bytes());
}

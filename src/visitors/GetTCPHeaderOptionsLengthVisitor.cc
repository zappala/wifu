

#include "visitors/GetTCPHeaderOptionsLengthVisitor.h"

GetTCPHeaderOptionsLengthVisitor::GetTCPHeaderOptionsLengthVisitor() : length_bytes_(0) {
    
}

u_int8_t GetTCPHeaderOptionsLengthVisitor::get_padded_length() {
    return ceil((double) length_bytes_ / 4.0);
}

void GetTCPHeaderOptionsLengthVisitor::visit(TCPHeaderOption* option) {
    length_bytes_ += option->get_length();
}

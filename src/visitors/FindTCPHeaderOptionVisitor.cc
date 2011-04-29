#include "visitors/FindTCPHeaderOptionVisitor.h"

FindTCPHeaderOptionVisitor::FindTCPHeaderOptionVisitor(u_int8_t kind) : Visitor(), kind_(kind), option_(0) {

}

void FindTCPHeaderOptionVisitor::visit(TCPHeaderOption* option) {
    if(option->get_kind() == kind_) {
        option_ = option;
    }
}

bool FindTCPHeaderOptionVisitor::stop() {
    return option_ != 0;
}

TCPHeaderOption* FindTCPHeaderOptionVisitor::get_option() {
    return option_;
}



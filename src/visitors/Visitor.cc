#include "visitors/Visitor.h"


Visitor::Visitor() {
    
}

Visitor::~Visitor() {
}

void Visitor::visit(Socket* s) {
    // Should be calling visit on an child class
    throw IllegalStateException();
}

void Visitor::visit(TCPHeaderOption* option) {
    // Should be calling visit on an child class
    throw IllegalStateException();
}

bool Visitor::stop() {
    return false;
}

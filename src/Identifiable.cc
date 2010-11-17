#include "Identifiable.h"

Identifiable::Identifiable() : id_(IDGenerator::instance().next_id()) {

}

Identifiable::~Identifiable() {
    IDGenerator::instance().remove_id(id_);
}

int& Identifiable::get_id() {
    return id_;
}

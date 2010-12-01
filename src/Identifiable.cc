#include "Identifiable.h"

Identifiable::Identifiable() : id_(IDGenerator::instance().next()) {

}

Identifiable::~Identifiable() {
    IDGenerator::instance().remove(id_);
}

int Identifiable::get_id() const {
    return id_;
}

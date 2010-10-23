#include "IDGenerator.h"

// private

IDGenerator::IDGenerator() {

}

// public

IDGenerator::~IDGenerator() {

}

IDGenerator & IDGenerator::instance() {
    static IDGenerator instance_;
    return instance_;
}

int IDGenerator::next_id() {

    int random;
    while(ids_.find(random = rand()) != ids_.end());
    
    ids_.insert(random);

    return random;
}

void IDGenerator::remove_id(int id) {
    ids_.erase(id);
}


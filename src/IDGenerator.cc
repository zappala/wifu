#include "IDGenerator.h"

// private

IDGenerator::IDGenerator() : NumberGenerator<int>() {

}

// public

IDGenerator::~IDGenerator() {

}

IDGenerator & IDGenerator::instance() {
    static IDGenerator instance_;
    return instance_;
}
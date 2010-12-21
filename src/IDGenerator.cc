#include "IDGenerator.h"

// private

IDGenerator::IDGenerator() : RandomNumberSet<int>() {

}

// public

IDGenerator::~IDGenerator() {

}

IDGenerator & IDGenerator::instance() {
    static IDGenerator instance_;
    return instance_;
}

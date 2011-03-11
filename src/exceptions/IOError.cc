#include "exceptions/IOError.h"

const char* IOError::what() const throw () {
    return "Error in performing I/O operation.";
}

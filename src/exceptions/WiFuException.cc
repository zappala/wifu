/*
 * WiFuException.cc
 *
 *  Created on: Mar 29, 2011
 *      Author: erickson
 */

#include "exceptions/WiFuException.h"

WiFuException::WiFuException(const char* message) : message_(message) {

}

const char* WiFuException::what() const throw () {
    return message_;
}

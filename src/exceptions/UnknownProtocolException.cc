/*
 * UnknownProtocolException.cc
 *
 *  Created on: Mar 24, 2011
 *      Author: erickson
 */

#include "exceptions/UnknownProtocolException.h"

const char* UnknownProtocolException::what() const throw() {
	return "Unknown protocol";
}

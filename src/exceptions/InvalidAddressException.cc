#include "InvalidAddressException.h"

const char* InvalidAddressException::what() const throw() {
	return "Invalid address";
}

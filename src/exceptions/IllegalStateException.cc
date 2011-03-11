#include "exceptions/IllegalStateException.h"

const char* IllegalStateException::what() const throw() {
	return "Illegal State";
}

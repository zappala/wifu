#ifndef INVALIDADDRESSEXPECTION
#define INVALIDADDRESSEXPECTION

#include <exception>

using namespace std;

class InvalidAddressException : public exception {
	virtual const char* what() const throw();
};

#endif

#ifndef INVALIDADDRESSEXPECTION
#define INVALIDADDRESSEXPECTION

#include <exception>

using namespace std;

class InvalidAddressException : public exception {
public:
	virtual const char* what() const throw();
};

#endif

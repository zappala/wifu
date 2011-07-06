#ifndef INVALIDADDRESSEXPECTION
#define INVALIDADDRESSEXPECTION

#include <exception>

using namespace std;

/**
 * Thrown when an address/port combination is invalid.
 */
class InvalidAddressException : public exception {
public:
    /**
     * @return InvalidAddressException specific error message.
     */
    virtual const char* what() const throw ();
};

#endif

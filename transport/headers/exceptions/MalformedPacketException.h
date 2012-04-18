#ifndef MALFORMEDPACKETEXCEPTION
#define MALFORMEDPACKETEXCEPTION

#include <exception>

using namespace std;

/**
 * Thrown when a WiFuPacket is found to be malformed.
 */
class MalformedPacketException : public exception {
public:
    /**
     * @return MalformedPacketException specific error message.
     */
    virtual const char* what() const throw ();
};

#endif

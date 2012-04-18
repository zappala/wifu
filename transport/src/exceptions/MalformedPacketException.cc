#include "exceptions/MalformedPacketException.h"

const char* MalformedPacketException::what() const throw () {
    return "The length has not been set.  Probably because IPPacket::set_data() has not been called.";
}

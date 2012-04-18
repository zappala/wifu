/*
 * PacketProtocolNotSetException.h
 *
 *  Created on: Mar 24, 2011
 *      Author: erickson
 */

#ifndef PACKETPROTOCOLNOTSETEXCEPTION_H_
#define PACKETPROTOCOLNOTSETEXCEPTION_H_

#include <exception>

using namespace std;

/**
 * Thrown when we try to access the IPPacket's protocol has not been set and we are trying to read a log.
 */
class PacketProtocolNotSetException : public exception {
public:
    /**
     * @return PacketProtocolNotSetException specific error message.
     */
    virtual const char* what() const throw ();
};

#endif /* PACKETPROTOCOLNOTSETEXCEPTION_H_ */

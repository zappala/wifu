/*
 * PacketProtocolNotSetException.cc
 *
 *  Created on: Mar 24, 2011
 *      Author: erickson
 */

#include "exceptions/PacketProtocolNotSetException.h"

const char* PacketProtocolNotSetException::what() const throw() {
	return "IPPacket's protocol field is not set";
}

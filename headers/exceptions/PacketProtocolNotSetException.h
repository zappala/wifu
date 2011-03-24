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

class PacketProtocolNotSetException : public exception {
public:
	virtual const char* what() const throw();
};

#endif /* PACKETPROTOCOLNOTSETEXCEPTION_H_ */

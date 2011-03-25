/*
 * UnknownProtocolException.h
 *
 *  Created on: Mar 24, 2011
 *      Author: erickson
 */

#ifndef UNKNOWNPROTOCOLEXCEPTION_H_
#define UNKNOWNPROTOCOLEXCEPTION_H_

#include <exception>

using namespace std;

class UnknownProtocolException : public exception {
public:
	virtual const char* what() const throw();
};

#endif /* UNKNOWNPROTOCOLEXCEPTION_H_ */

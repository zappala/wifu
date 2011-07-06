/*
 * WiFuException.h
 *
 *  Created on: Mar 29, 2011
 *      Author: erickson
 */

#ifndef WIFUEXCEPTION_H_
#define WIFUEXCEPTION_H_

#include <exception>
#include <string>

using namespace std;

/**
 * General exception with customizable message.
 */
class WiFuException : public exception {
public:
    /**
     * Constructs a WiFuException
     * @param message The message to return if this exception is thrown.
     */
    WiFuException(const char* message);

    /**
     * @return A customized message.
     */
    virtual const char* what() const throw ();

private:

    /**
     * Custom message to throw.
     */
    const char* message_;

};


#endif /* WIFUEXCEPTION_H_ */

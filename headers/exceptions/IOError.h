/* 
 * File:   IOError.h
 * Author: rbuck
 *
 * Created on March 11, 2011, 2:37 PM
 */

#ifndef _IOERROR_H
#define	_IOERROR_H

#include <exception>

using namespace std;

/**
 * Represents an I/O exception.
 */
class IOError : public exception {
public:
    /**
     * @return IOError specific error message.
     */
    virtual const char* what() const throw ();
};

#endif	/* _IOERROR_H */


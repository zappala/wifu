/* 
 * File:   IllegalStateException.h
 * Author: rbuck
 *
 * Created on March 11, 2011, 10:45 AM
 */

#ifndef _ILLEGALSTATEEXCEPTION_H
#define	_ILLEGALSTATEEXCEPTION_H

#include <exception>

using namespace std;

/**
 * Thrown when we are in an illegal state.
 */
class IllegalStateException : public exception {
public:

    /**
     * @return IllegalStateException specific error message.
     */
    virtual const char* what() const throw ();
};

#endif	/* _ILLEGALSTATEEXCEPTION_H */


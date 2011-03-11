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

class IllegalStateException : public exception {
public:
	virtual const char* what() const throw();
};

#endif	/* _ILLEGALSTATEEXCEPTION_H */


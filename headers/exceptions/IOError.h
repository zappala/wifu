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

class IOError : public exception {
public:
	virtual const char* what() const throw();
};

#endif	/* _IOERROR_H */


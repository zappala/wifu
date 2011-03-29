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

class WiFuException : public exception {
public:
	WiFuException(const char* message);

	virtual const char* what() const throw();

private:
	const char* message_;

};


#endif /* WIFUEXCEPTION_H_ */

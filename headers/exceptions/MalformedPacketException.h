#ifndef MALFORMEDPACKETEXCEPTION
#define MALFORMEDPACKETEXCEPTION

#include <exception>

using namespace std;

class MalformedPacketException : public exception {
public:
	virtual const char* what() const throw();
};

#endif

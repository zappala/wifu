/* 
 * File:   Message.h
 * Author: rbuck
 *
 * Created on October 20, 2010, 11:07 AM
 */

#ifndef _MESSAGE_H
#define	_MESSAGE_H

#include <string>
#include <cstring>

using namespace std;

class Message {
public:
    Message(int size, char* message);
    Message(int size, string& message);

    // Copy constructor
    // Assignment operator

    virtual ~Message();
    string to_s();
    char* c_str();

private:
    int size_;
    string string_;
    char* c_string_;
};

#endif	/* _MESSAGE_H */


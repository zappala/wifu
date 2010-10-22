/* 
 * File:   Message.h
 * Author: rbuck
 *
 * Created on October 20, 2010, 11:07 AM
 */

#ifndef _MESSAGE_H
#define	_MESSAGE_H

#include <string>

using namespace std;

class Message{
public:
    Message(int size, char * message) {
        strcpy(c_string_, message);
    }

    Message(int size, string & message) {
        string_ = message;
        c_string_ = 0;
    }

    // Copy constructor
    // Assignment operator

    ~Message();

    string to_s() {
        if(c_string_) {
            return string(c_string_);
        }
        return string_;
    }

    char * c_str() {
        if(c_string_) {
            return string(c_string_);
        }
        return string_.c_str();
    }
    

private:
    int size_;
    string string_;
    char * c_string_;
    
};

#endif	/* _MESSAGE_H */


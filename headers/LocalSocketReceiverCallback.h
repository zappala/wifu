/* 
 * File:   LocalSocketReceiverCallback.h
 * Author: rbuck
 *
 * Created on October 19, 2010, 2:43 PM
 */

#ifndef _LOCALSOCKETRECEIVERCALLBACK_H
#define	_LOCALSOCKETRECEIVERCALLBACK_H

#include <string>

using namespace std;

class LocalSocketReceiverCallback {
public:
    virtual void receive(string &  message) = 0;
};

#endif	/* _LOCALSOCKETRECEIVERCALLBACK_H */


/* 
 * File:   LocalSocketReceiverCallback.h
 * Author: rbuck
 *
 * Created on October 19, 2010, 2:43 PM
 */

#ifndef _LOCALSOCKETRECEIVERCALLBACK_H
#define	_LOCALSOCKETRECEIVERCALLBACK_H

#include "defines.h"

using namespace std;

class LocalSocketReceiverCallback {
public:

    /**
     * Callback function used to process messages received by a Unix socket.
     *
     * @param message Message received from a Unix socket.
     * @param length Length of message in bytes.
     */
    virtual void receive(unsigned char* message, int length) = 0;
};

#endif	/* _LOCALSOCKETRECEIVERCALLBACK_H */


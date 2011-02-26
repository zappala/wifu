/* 
 * File:   BaseModule.h
 * Author: rbuck
 *
 * Created on October 18, 2010, 1:48 PM
 */

#ifndef _LOCALSOCKETFULLDUPLEX_H
#define	_LOCALSOCKETFULLDUPLEX_H

#include <string>

#include "LocalSocketSender.h"
#include "LocalSocketReceiver.h"
#include "LocalSocketReceiverCallback.h"

using namespace std;

/**
 * Full-duplex unix socket
 * This class reads from one unix socket and write to any number of other unix sockets (given that it knows about it)
 *
 * @see LocalSocketSender
 * @see LocalSocketReceiver
 * @see LocalSocketReceiverCallback
 */
class LocalSocketFullDuplex : public LocalSocketReceiverCallback, public LocalSocketSender, public LocalSocketReceiver {
public:

    /**
     * Constructor.  Simply calls the Constructors of the classes it inherits from.
     *
     * @param file The file which will be used to create the underlying unix socket to read from.
     *
     * @see LocalSocketSender
     * @see LocalSocketReceiver
     * @see LocalSocketReceiverCallback
     */
    LocalSocketFullDuplex(string & file);

    /**
     * Constructor.  Simply calls the Constructors of the classes it inherits from.
     *
     * @param file The file which will be used to create the underlying unix socket to read from.
     *
     * @see LocalSocketSender
     * @see LocalSocketReceiver
     * @see LocalSocketReceiverCallback
     */
    LocalSocketFullDuplex(const char* file);

    /**
     * Cleans up this LocalSocketFullDuplex object
     */
    virtual ~LocalSocketFullDuplex();

};

#endif	/* _LOCALSOCKETFULLDUPLEX_H */


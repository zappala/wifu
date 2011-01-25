/* 
 * File:   SocketManager.h
 * Author: rbuck
 *
 * Created on December 3, 2010, 5:06 PM
 */

#ifndef _SOCKETMANAGER_H
#define	_SOCKETMANAGER_H

#include "RandomNumberSet.h"
#include <stdint.h>

class SocketManager : public RandomNumberSet<uint16_t> {
private:

    SocketManager() : RandomNumberSet<uint16_t>() {
        // We cannot have a socket with id 0 because we use 0
        // As the default socket on the front end
        // see wifu_socket() for details.
        insert(0);
    }

public:

    virtual ~SocketManager() {

    }

    static SocketManager& instance() {
        static SocketManager instance_;
        return instance_;
    }
};

#endif	/* _SOCKETMANAGER_H */


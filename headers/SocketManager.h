/* 
 * File:   SocketManager.h
 * Author: rbuck
 *
 * Created on December 3, 2010, 5:06 PM
 */

#ifndef _SOCKETMANAGER_H
#define	_SOCKETMANAGER_H

#include "NumberGenerator.h"

class SocketManager : public NumberGenerator<uint16_t> {
private:

    SocketManager() : NumberGenerator<uint16_t>() {

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


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
    SocketManager();

public:
    virtual ~SocketManager();

    static SocketManager& instance();
};

#endif	/* _SOCKETMANAGER_H */


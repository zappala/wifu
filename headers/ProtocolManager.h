/* 
 * File:   ProtocolManager.h
 * Author: rbuck
 *
 * Created on December 22, 2010, 10:10 AM
 */

#ifndef _PROTOCOLMANAGER_H
#define	_PROTOCOLMANAGER_H

#include "HashSet.h"
#include "Socket.h"

/**
 * Keeps track of the set of protocols (ints) which is supported by
 * wifu end.
 */
class ProtocolManager : public HashSet<int> {
private:
    ProtocolManager();

    ProtocolManager(ProtocolManager const&);

    ProtocolManager& operator=(ProtocolManager const&);

    Semaphore mutex_;

public:
    virtual ~ProtocolManager();

    static ProtocolManager& instance();

    bool is_supported(int, int, int);

    void support(int);
};

#endif	/* _PROTOCOLMANAGER_H */


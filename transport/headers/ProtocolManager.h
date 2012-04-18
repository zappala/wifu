/* 
 * File:   ProtocolManager.h
 * Author: rbuck
 *
 * Created on December 22, 2010, 10:10 AM
 */

#ifndef _PROTOCOLMANAGER_H
#define	_PROTOCOLMANAGER_H

#include "Set.h"
#include "Socket.h"

/**
 * Keeps track of the set of protocols (ints) which is supported by
 * WiFu Transport.
 */
class ProtocolManager : public Set<int> {
private:
    ProtocolManager();

    ProtocolManager(ProtocolManager const&);

    ProtocolManager& operator=(ProtocolManager const&);

    Semaphore mutex_;

public:
    virtual ~ProtocolManager();

    static ProtocolManager& instance();

    bool is_supported(int domain, int type, int protocol);

    void support(int protocol);
};

#endif	/* _PROTOCOLMANAGER_H */


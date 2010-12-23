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
    ProtocolManager() {}
    ProtocolManager(ProtocolManager const&){}
    ProtocolManager& operator=(ProtocolManager const&){}

public:
    virtual ~ProtocolManager() {}

    static ProtocolManager& instance() {
        static ProtocolManager instance_;
        return instance_;
    }

    bool is_supported(int domain, int type, int protocol) {
        // TODO: support checking the three-tuple:
        // TODO: domain, type, protocol
        // TODO: It is possible that the protocol will be 0,
        // TODO: in which case it is up to this class to
        // TODO: determine whether or not it is supported
        // TODO: This means that the class will need to map the
        // TODO: three-tuple to a protocol
        return contains(protocol);
    }

    void support(int protocol) {
        insert(protocol);
    }

};

#endif	/* _PROTOCOLMANAGER_H */


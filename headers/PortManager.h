/* 
 * File:   PortManager.h
 * Author: rbuck
 *
 * Created on December 1, 2010, 3:03 PM
 */

#ifndef _PORTMANAGER_H
#define	_PORTMANAGER_H

#include "NumberGenerator.h"

class PortManager : public NumberGenerator<uint16_t> {
private:

    PortManager() : NumberGenerator<uint16_t>() {}

public:

    virtual ~PortManager() {}
    
    static PortManager& instance() {
        static PortManager instance_;
        return instance_;
    }
};

#endif	/* _PORTMANAGER_H */


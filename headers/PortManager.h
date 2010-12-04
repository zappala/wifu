/* 
 * File:   PortManager.h
 * Author: rbuck
 *
 * Created on December 1, 2010, 3:03 PM
 */

#ifndef _PORTMANAGER_H
#define	_PORTMANAGER_H

#include "ShortGenerator.h"

class PortManager : public ShortGenerator {
private:

    PortManager() : ShortGenerator() {

    }

public:

    virtual ~PortManager() {

    }
    
    static PortManager& instance() {
        static PortManager instance_;
        return instance_;
    }
};

#endif	/* _PORTMANAGER_H */


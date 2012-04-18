/* 
 * File:   PortManager.h
 * Author: rbuck
 *
 * Created on December 1, 2010, 3:03 PM
 */

#ifndef _PORTMANAGER_H
#define	_PORTMANAGER_H

#include <stdint.h>

#include "RandomNumberSet.h"
#include "IPortManager.h"

class PortManager : public IPortManager, public RandomNumberSet<uint16_t> {
private:
    PortManager();

public:
    virtual ~PortManager();
    static PortManager& instance();
    
    virtual u_int16_t get();
    virtual void remove(u_int16_t port);
    virtual void reset();
};

#endif	/* _PORTMANAGER_H */


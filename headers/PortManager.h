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

class PortManager : public RandomNumberSet<uint16_t> {
private:
    PortManager();

public:
    virtual ~PortManager();
    static PortManager& instance();
};

#endif	/* _PORTMANAGER_H */


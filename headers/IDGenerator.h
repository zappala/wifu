/* 
 * File:   TimeoutEventManagerSingleton.h
 * Author: rbuck
 *
 * Created on October 23, 2010, 9:36 AM
 */

#ifndef _IDGENERATOR_H
#define	_IDGENERATOR_H

#include "NumberGenerator.h"

/**
 * Generates globally unique ids.
 */
class IDGenerator : public NumberGenerator<int> {
private:

    /**
     * Constructs an IDGenerator object.
     */
    IDGenerator();

public:

    /**
     * Cleans up this IDGenerator.
     */
    virtual ~IDGenerator();

    /**
     * @return A reference to the static instance of this IDGenerator object.
     */
    static IDGenerator & instance();
    
};


#endif	/* _TIMEOUTEVENTMANAGERSEMAPHORE_H */

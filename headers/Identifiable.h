/* 
 * File:   Identifiable.h
 * Author: rbuck
 *
 * Created on October 23, 2010, 11:14 AM
 */

#ifndef _IDENTIFIABLE_H
#define	_IDENTIFIABLE_H

#include "IDGenerator.h"

/**
 * Keeps a globally unique id.
 *
 * @see IDGenerator
 */
class Identifiable {
public:

    /**
     * Constructs an Identifiable object.
     * Calls IDGenerator::next_id() and stores it.
     */
    Identifiable();

    /**
     * Cleans up this Identifiable object.
     * Calls IDGenerator::remove_id() and thus releases the id used by this object.
     */
    virtual ~Identifiable();

    /**
     * @return The id of this object.
     */
    int & get_id();
    
private:
    /**
     * Unique id of this Identifiable object.
     */
    int id_;
};

#endif	/* _IDENTIFIABLE_H */


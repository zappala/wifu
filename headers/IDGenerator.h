/* 
 * File:   TimeoutEventManagerSingleton.h
 * Author: rbuck
 *
 * Created on October 23, 2010, 9:36 AM
 */

#ifndef _IDGENERATOR_H
#define	_IDGENERATOR_H

#include <tr1/unordered_set>
#include <cstdlib>
#include <time.h>

using namespace std;

/**
 * Generates globally unique ids.
 */
class IDGenerator {
private:

    /**
     * Constructs an IDGenerator object.
     */
    IDGenerator();
    
    tr1::unordered_set<int> ids_;

public:

    /**
     * Cleans up this IDGenerator.
     */
    virtual ~IDGenerator();

    /**
     * @return A reference to the static instance of this IDGenerator object.
     */
    static IDGenerator & instance();

    /**
     * Generates a globally unique int.
     *
     * @return A globally unique int.
     */
    int next_id(void);

    /**
     * Returns id to the pool of possible ids.
     *
     * @param id The id to remove from the set of in-use ids.
     */
    void remove_id(int& id);
};


#endif	/* _TIMEOUTEVENTMANAGERSEMAPHORE_H */

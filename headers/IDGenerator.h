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

class IDGenerator {
private:
    IDGenerator();
    
    tr1::unordered_set<int> ids_;


public:
    virtual ~IDGenerator();
    static IDGenerator & instance();

    int next_id(void);
    void remove_id(int& id);
};


#endif	/* _TIMEOUTEVENTMANAGERSEMAPHORE_H */

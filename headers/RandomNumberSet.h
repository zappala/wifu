/* 
 * File:   RandomNumberSet.h
 * Author: rbuck
 *
 * Created on December 1, 2010, 3:17 PM
 */

#ifndef _NUMBERGENERATOR_H
#define	_NUMBERGENERATOR_H

#include <cstdlib>
#include <time.h>

#include "HashSet.h"

using namespace std;

template<class N>
class RandomNumberSet : public HashSet<N> {
public:

    RandomNumberSet() {
        reset_seed();
    }

    virtual ~RandomNumberSet() {

    }

    virtual N get() {
        N random;

        do {
            random = rand();
        } while (contains(random));

        // TODO: ensure that we are removing these ids at some point
        insert(random);
        return random;
    }

    void reset_seed() {
        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        srand(ts.tv_nsec);
    }

};

#endif	/* _NUMBERGENERATOR_H */


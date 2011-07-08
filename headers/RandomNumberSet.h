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

#include "Set.h"

using namespace std;

template<class N>
class RandomNumberSet : public Set<N> {
public:

    RandomNumberSet() {
        mutex_.init(1);
        reset_seed();
    }

    virtual ~RandomNumberSet() {

    }

    virtual N get() {
        mutex_.wait();
        N random;

        do {
            random = rand();
        } while (contains(random));

        // TODO: ensure that we are removing these ids at some point
        insert(random);
        mutex_.post();
        return random;
    }

    void reset_seed() {
        mutex_.wait();
        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        srand(ts.tv_nsec);
        mutex_.post();
    }

private:
    Semaphore mutex_;

};

#endif	/* _NUMBERGENERATOR_H */


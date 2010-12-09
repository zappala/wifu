/* 
 * File:   NumberGenerator.h
 * Author: rbuck
 *
 * Created on December 1, 2010, 3:17 PM
 */

#ifndef _NUMBERGENERATOR_H
#define	_NUMBERGENERATOR_H

#include <tr1/unordered_set>
#include <cstdlib>
#include <time.h>

using namespace std;

template<class N>
class NumberGenerator {
public:

    NumberGenerator() {
        reset_seed();
    }

    virtual ~NumberGenerator() {

    }

    virtual N next() {
        N random;
        while (ids_.find(random = rand()) != ids_.end());
        use(random);
        return random;
    }

    void use(N number) {
        ids_.insert(number);
    }

    void remove(N number) {
        ids_.erase(number);
    }

    void reset_seed() {
        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        srand(ts.tv_nsec);
    }

protected:
    tr1::unordered_set<N> ids_;
};

#endif	/* _NUMBERGENERATOR_H */


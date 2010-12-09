/* 
 * File:   ShortGenerator.h
 * Author: rbuck
 *
 * Created on December 3, 2010, 4:57 PM
 */

#ifndef _SHORTGENERATOR_H
#define	_SHORTGENERATOR_H

#include "NumberGenerator.h"
#include <cmath>

class ShortGenerator : public NumberGenerator<uint16_t> {
public:
    ShortGenerator() : NumberGenerator<uint16_t>(), max_(pow(2, 15)) {

    }

    virtual ~ShortGenerator() {

    }

    uint16_t next() {
        reset_seed();
        
        uint16_t random;

        while (true) {
            random = rand();

            // Ensure the left-most bit is 0
            random &= 0x7FFF;

            assert(!(random & 0x8000));
            assert(random <= max_);

            if(ids_.find(random) == ids_.end()) {
                break;
            }
        }
        use(random);
        return random;
    }

private:
    uint16_t max_;
};

#endif	/* _SHORTGENERATOR_H */


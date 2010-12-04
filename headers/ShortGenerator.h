/* 
 * File:   ShortGenerator.h
 * Author: rbuck
 *
 * Created on December 3, 2010, 4:57 PM
 */

#ifndef _SHORTGENERATOR_H
#define	_SHORTGENERATOR_H

#include "NumberGenerator.h"

class ShortGenerator : public NumberGenerator<uint16_t> {
public:
    ShortGenerator() : NumberGenerator<uint16_t>() {

    }

    virtual ~ShortGenerator() {

    }

    uint16_t next() {
        uint16_t random;

        while (true) {
            random = rand();
            random &= 0x7FFF;
            if(ids_.find(random = rand()) == ids_.end()) {
                break;
            }
        }
        use(random);
        cout << "Random Short: " << random << endl;
        return random;
    }
};

#endif	/* _SHORTGENERATOR_H */


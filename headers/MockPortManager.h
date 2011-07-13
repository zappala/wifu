/* 
 * File:   MockPortManager.h
 * Author: rbuck
 *
 * Created on April 11, 2011, 5:34 PM
 */

#ifndef MOCKPORTMANAGER_H
#define	MOCKPORTMANAGER_H

#include <stdint.h>

#include "RandomNumberSet.h"
#include "IPortManager.h"

class MockPortManager : public IPortManager, public RandomNumberSet<u_int16_t> {
private:
    MockPortManager();

public:
    virtual ~MockPortManager();
    static MockPortManager& instance();

    virtual u_int16_t get();
    virtual void remove(u_int16_t port);
    virtual void reset();
};

#endif	/* MOCKPORTMANAGER_H */


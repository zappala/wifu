/* 
 * File:   PortManagerFactory.h
 * Author: rbuck
 *
 * Created on April 11, 2011, 4:59 PM
 */

#ifndef PORTMANAGERFACTORY_H
#define	PORTMANAGERFACTORY_H

#include "IPortManagerCreator.h"

class PortManagerFactory {
public:
    static PortManagerFactory& instance() {
        static PortManagerFactory instance_;
        return instance_;
    }

    void set_creator(IPortManagerCreator* creator) {
        creator_ = creator;
    }

    IPortManager& create() {
        return creator_->create();
    }

private:

    PortManagerFactory() {}
    IPortManagerCreator* creator_;
};

#endif	/* PORTMANAGERFACTORY_H */


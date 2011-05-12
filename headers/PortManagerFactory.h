/* 
 * File:   PortManagerFactory.h
 * Author: rbuck
 *
 * Created on April 11, 2011, 4:59 PM
 */

#ifndef PORTMANAGERFACTORY_H
#define	PORTMANAGERFACTORY_H

#include "IPortManager.h"
#include "IPortManagerCreator.h"


class PortManagerFactory {
public:
    static PortManagerFactory& instance();
    void set_creator(IPortManagerCreator* creator);
    IPortManager& create();
private:

    PortManagerFactory();
    IPortManagerCreator* creator_;
};

#endif	/* PORTMANAGERFACTORY_H */


/* 
 * File:   ContextContainer.h
 * Author: rbuck
 *
 * Created on December 31, 2010, 9:52 AM
 */

#ifndef CONTEXTCONTAINER_H
#define	CONTEXTCONTAINER_H

#include "IContext.h"
#include "GarbageCollector.h"
#include "ReliabilityContext.h"
#include "CongestionControlContext.h"
#include "ConnectionManagerContext.h"

using namespace std;

class IContextContainer : public gc {
public:
    IContextContainer();

    IContextContainer(IContextContainer const& other);

    IContext* get_congestion_control();

    IContext* get_reliability();

    IContext* get_connection_manager();

private:
    IContext* reliability_;
    IContext* cc_;
    IContext* cm_;

};

#endif	/* CONTEXTCONTAINER_H */

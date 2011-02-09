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
    IContextContainer() {
        reliability_ = new ReliabilityContext();
        cc_ = new CongestionControlContext();
        cm_ = new ConnectionManagerContext();
    }

    IContext* get_congestion_control() {
        return cc_;
    }

    IContext* get_reliability() {
        return reliability_;
    }

    IContext* get_connection_manager() {
        return cm_;
    }

private:
    IContext* reliability_;
    IContext* cc_;
    IContext* cm_;

};

#endif	/* CONTEXTCONTAINER_H */


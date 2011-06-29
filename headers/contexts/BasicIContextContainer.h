/* 
 * File:   BasicIContextContainer.h
 * Author: rbuck
 *
 * Created on June 28, 2011, 4:35 PM
 */

#ifndef BASICICONTEXTCONTAINER_H
#define	BASICICONTEXTCONTAINER_H

#include "GarbageCollector.h"
#include "IContext.h"

class BasicIContextContainer : public gc {
public:
    BasicIContextContainer();
    virtual ~BasicIContextContainer();
    
    IContext* get_connection_manager() const;
    void set_connection_manager(IContext* connection_manager);

    IContext* get_congestion_control() const;
    void set_congestion_control(IContext* congestion_control);

    IContext* get_reliability() const;
    void set_reliability(IContext* reliability);

private:
    IContext* connection_manager_;
    IContext* congestion_control_;
    IContext* reliability_;
};

#endif	/* BASICICONTEXTCONTAINER_H */


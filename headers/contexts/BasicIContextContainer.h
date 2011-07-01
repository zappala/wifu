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

/**
 * Stores three different IContect pointers.
 * Provides getters and setters for a connection manager, reliabiliy, and congestion control IContext implementation.
 *
 * @see IContext
 * @see TCPTahoe
 */
class BasicIContextContainer : public gc {
public:

    /**
     * Contructor.
     */
    BasicIContextContainer();

    /**
     * Destructor.
     */
    virtual ~BasicIContextContainer();

    /**
     * @return A pointer to a connection manager IContext.
     */
    IContext* get_connection_manager() const;

    /**
     * Sets the internal connection manager pointer to connection_manager.
     * @param connection_manager The pointer to set the internal connection manager pointer to.
     */
    void set_connection_manager(IContext* connection_manager);

    /**
     * @return A pointer to a  congestion controll IContext.
     */
    IContext* get_congestion_control() const;

    /**
     * Sets the internal connection manager pointer to congestion_control.
     * @param connection_manager The pointer to set the internal connection manager pointer to.
     */
    void set_congestion_control(IContext* congestion_control);

    /**
     * @return A pointer to a reliability IContext.
     */
    IContext* get_reliability() const;

    /**
     * Sets the internal connection manager pointer to connection_manager.
     * @param connection_manager The pointer to set the internal connection manager pointer to.
     */
    void set_reliability(IContext* reliability);

private:
    /**
     * IContext pointer to the connection_manager.
     */
    IContext* connection_manager_;
    
    /**
     * IContext pointer to the congestion_controller.
     */
    IContext* congestion_control_;

    /**
     * IContext pointer to reliability.
     */
    IContext* reliability_;
};

#endif	/* BASICICONTEXTCONTAINER_H */


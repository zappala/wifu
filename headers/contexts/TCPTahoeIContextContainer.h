/* 
 * File:   TCPTahoeIContextContainer.h
 * Author: rbuck
 *
 * Created on May 4, 2011, 3:55 PM
 */

#ifndef TCPTAHOEICONTEXTCONTAINER_H
#define	TCPTAHOEICONTEXTCONTAINER_H

#include "contexts/ConnectionManagerContext.h"

class TCPTahoeIContextContainer : public gc {
public:
    TCPTahoeIContextContainer();
    ~TCPTahoeIContextContainer();

    IContext* get_connection_manager();

private:
    IContext* cm_;
};

#endif	/* TCPTAHOEICONTEXTCONTAINER_H */


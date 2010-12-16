/* 
 * File:   CloseWait.h
 * Author: rbuck
 *
 * Created on September 28, 2010, 9:41 AM
 */

#ifndef _CLOSEWAIT_H
#define	_CLOSEWAIT_H

#include "Connection.h"


class CloseWait : public TCPState {
public:
    CloseWait();
    ~CloseWait();

    virtual void close(Connection * c);
};


#endif	/* _CLOSEWAIT_H */


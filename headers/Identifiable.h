/* 
 * File:   Identifiable.h
 * Author: rbuck
 *
 * Created on October 23, 2010, 11:14 AM
 */

#ifndef _IDENTIFIABLE_H
#define	_IDENTIFIABLE_H

#include "IDGenerator.h"

class Identifiable {
public:
    Identifiable();
    virtual ~Identifiable();
    int & get_id();
private:
    int id_;
};

#endif	/* _IDENTIFIABLE_H */


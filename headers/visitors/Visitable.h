/* 
 * File:   Visitable.h
 * Author: rbuck
 *
 * Created on December 24, 2010, 9:35 AM
 */

#ifndef VISITABLE_H
#define	VISITABLE_H

#include "Visitor.h"

class Visitable {
public:
    virtual ~Visitable() {}
    virtual void accept(Visitor* v) = 0;
};

#endif	/* VISITABLE_H */


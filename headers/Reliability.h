/* 
 * File:   Reliability.h
 * Author: rbuck
 *
 * Created on October 18, 2010, 2:26 PM
 */

#ifndef _RELIABILITY_H
#define	_RELIABILITY_H

#include <iostream>

#include "LocalSocketFullDuplex.h"
#include "IModule.h"


class Reliability : public LocalSocketFullDuplex, public IModule {
public:
    Reliability(string & file);
    virtual ~Reliability();

    void receive(string &  message);

    void connect(string & message);
    void data(string & message);
};


#endif	/* _RELIABILITY_H */


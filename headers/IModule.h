/* 
 * File:   IModule.h
 * Author: rbuck
 *
 * Created on October 20, 2010, 12:34 PM
 */

#ifndef _IMODULE_H
#define	_IMODULE_H

#include <string>

using namespace std;

class IModule {
public:
    virtual ~IModule() {
        
    }
    virtual void connect(string & message) = 0;
    virtual void data(string & message) = 0;
    
};

#endif	/* _IMODULE_H */


/* 
 * File:   IModule.h
 * Author: rbuck
 *
 * Created on October 20, 2010, 12:34 PM
 */

#ifndef _ISOCKETMODULE_H
#define	_ISOCKETMODULE_H

#include <string>

using namespace std;

class ISocketModule {
public:
    virtual ~ISocketModule() {}
    virtual void connect(string & message) = 0;
    virtual void data(string & message) = 0;
    
};

#endif	/* _ISOCKETMODULE_H */


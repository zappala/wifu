/* 
 * File:   SlowStart.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 3:55 PM
 */

#ifndef SLOWSTART_H
#define	SLOWSTART_H

#include <string>

#include "../contexts/Context.h"
#include "../AddressPort.h"

using namespace std;

class SlowStart : public State {
public:
    SlowStart();
    virtual ~SlowStart();

    void receive(Context* c, string& data);
    void enter(Context* c);
    void exit(Context* c);
    void connect(Context* c, AddressPort& remote);
};


#endif	/* SLOWSTART_H */


/* 
 * File:   QConnectionManager.h
 * Author: rbuck
 *
 * Created on October 29, 2010, 2:12 PM
 */

#ifndef _QCONNECTIONMANAGER_H
#define	_QCONNECTIONMANAGER_H

#include "IQModule.h"
#include "ConnectionManager.h"
#include "ConnectEvent.h"

class QConnnectionManager : public IQModule {
public:

    QConnnectionManager() : IQModule() {
        s_ = "Connection Manager blah";
    }

    virtual ~QConnnectionManager() {

    }

    void process(Event* e) {
        e->execute(this);
    }

    void connect(Event* e) {
        ConnectEvent* c = (ConnectEvent*) e;
        cout << get_string() << endl;
    }

    void data(Event* e) {

    }

    string & get_string() {
        return s_;
    }

private:
    string s_;
};


#endif	/* _QCONNECTIONMANAGER_H */


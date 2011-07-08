/* 
 * File:   SocketCollection.h
 * Author: rbuck
 *
 * Created on December 21, 2010, 8:10 AM
 */

#ifndef _SOCKETCOLLECTION_H
#define	_SOCKETCOLLECTION_H

#include <algorithm>
#include <typeinfo>

#include "Socket.h"
#include "visitors/Visitable.h"
#include "visitors/Visitor.h"
#include "visitors/SocketCollectionGetByIdVisitor.h"
#include "visitors/SocketCollectionGetByLocalAddressPortVisitor.h"
#include "observer/Observer.h"

using namespace std;

class SocketCollection : public Visitable, public Observer {
private:

    SocketCollection();

    SocketCollection & operator=(const SocketCollection& other);

    SocketCollection(const SocketCollection& other);

    map<gcstring, Socket*, std::less<gcstring>, gc_allocator<std::pair<const gcstring, Socket*> > >collection_;
    map<gcstring, Socket*, std::less<gcstring>, gc_allocator<std::pair<const gcstring, Socket*> > >::iterator itr_;
    Semaphore* mutex_;
    Semaphore* update_mutex_;

public:

    static SocketCollection& instance();

    virtual ~SocketCollection();

    Socket* get_by_id(int id);

    Socket* get_by_local_ap(AddressPort* local);

    Socket* get_by_local_and_remote_ap(AddressPort* local, AddressPort* remote);

    void push(Socket* s);

    void remove(Socket* s);

    int size();

    int clear();

    virtual void accept(Visitor* v);

    void update(Observable* o);

    void reset();

};

#endif	/* _SOCKETCOLLECTION_H */

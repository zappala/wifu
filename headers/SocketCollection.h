/* 
 * File:   SocketCollection.h
 * Author: rbuck
 *
 * Created on December 21, 2010, 8:10 AM
 */

#ifndef _SOCKETCOLLECTION_H
#define	_SOCKETCOLLECTION_H

#include <algorithm>
#include <vector>

#include "Socket.h"
#include "visitors/Visitable.h"
#include "visitors/Visitor.h"
#include "observer/Observer.h"

using namespace std;

enum HowSorted {
    ID,
    AP,
    RE_SORT
};

// functions used in sorting

bool id_cmp(Socket* a, Socket* b);

bool ap_cmp(Socket* a, Socket* b);

// functions used in binary search

int bsearch_id_cmp(int& value, Socket* s);

int b_search_local_cmp(AddressPort* local, Socket* s);

// first element in pair is local, second is remote

int b_search_local_remote_cmp(pair<AddressPort*, AddressPort*>& aps, Socket* s);

class SocketCollection : public Visitable, public Observer {
private:

    SocketCollection();

    SocketCollection& operator=(const SocketCollection& other);

    SocketCollection(const SocketCollection& other);

    template<typename Type1, typename Type2>
    Socket* binary_search(Type1 target, int (*compare)(Type2, Socket*));

    void socket_sort(bool(*f)(Socket*, Socket*));

    void mark_dirty();

    vector<Socket*> collection_;
    Semaphore mutex_;
    HowSorted how_sorted_;

public:

    static SocketCollection& instance();

    virtual ~SocketCollection();

    Socket* get_by_id(int id);

    Socket* get_by_local_ap(AddressPort* local);

    Socket* get_by_local_and_remote_ap(AddressPort* local, AddressPort* remote);

    void push(Socket* s);

    int size();

    int clear();

    void shuffle();

    virtual void accept(Visitor* v);

    void update(Observable* o);

};

#endif	/* _SOCKETCOLLECTION_H */

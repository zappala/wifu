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

using namespace std;

enum HowSorted {
    ID,
    AP,
    RE_SORT
};

// functions used in sorting
bool id_cmp(Socket* a, Socket* b) {
    return a->get_socket() < b->get_socket();
}

bool ap_cmp(Socket* a, Socket* b) {
    string astring = a->get_local_address_port()->to_s();
    string bstring = b->get_local_address_port()->to_s();

    if (astring < bstring) {
        return true;
    } else if (astring == bstring) {
        return a->get_remote_address_port()->to_s() < b->get_remote_address_port()->to_s();
    }

    return false;
}

// functions used in binary search
int bsearch_id_cmp(int& value, Socket* s) {
    return value - s->get_socket();
}

int b_search_local_cmp(AddressPort* local, Socket* s) {
    return local->to_s().compare(s->get_local_address_port()->to_s());
}

// first element in pair is local, second is remote
int b_search_local_remote_cmp(pair<AddressPort*, AddressPort*>& aps, Socket* s) {
    AddressPort* local = aps.first;
    AddressPort* remote = aps.second;

    int value = local->to_s().compare(s->get_local_address_port()->to_s());
    if (value < 0) {
        return -1;
    } else if (value > 0) {
        return 1;
    }

    // locals are the same, compare remote
    return remote->to_s().compare(s->get_remote_address_port()->to_s());

}

class SocketCollection {
private:

    SocketCollection() : how_sorted_(RE_SORT) {
        mutex_.init(1);
    }

    SocketCollection & operator=(const SocketCollection& other) {
    }

    SocketCollection(const SocketCollection& other) {
    }

    template<typename Type, typename Type2>
    Socket* binary_search(Type target, int(*compare)(Type2, Socket*)) {
        typedef vector<Socket*>::iterator itr;
        itr begin = collection_.begin();
        itr end = collection_.end();

        while (begin < end) {
            itr middle = begin + (end - begin - 1) / 2;
            int value = compare(target, *middle);

            if (value < 0) {
                end = middle;
            } else if (value > 0) {
                begin = middle + 1;
            } else {
                return *middle;
            }
        }
        return NULL;
    }

    void socket_sort(bool(*f)(Socket*, Socket*)) {
        sort(collection_.begin(), collection_.end(), f);
    }

    vector<Socket*> collection_;
    Semaphore mutex_;
    HowSorted how_sorted_;

public:

    static SocketCollection& instance() {
        static SocketCollection instance_;
        return instance_;
    }

    virtual ~SocketCollection() {
    }

    Socket* get_by_id(int id) {
        mutex_.wait();
        if (how_sorted_ != ID) {
            socket_sort(id_cmp);
            how_sorted_ = ID;
        }
        Socket* value = binary_search(id, bsearch_id_cmp);
        mutex_.post();
        return value;
    }

    Socket* get_by_local_ap(AddressPort* local) {
        mutex_.wait();
        if (how_sorted_ != AP) {
            socket_sort(ap_cmp);
            how_sorted_ = AP;
        }

        Socket* value = binary_search(local, b_search_local_cmp);
        mutex_.post();
        return value;
    }

    Socket* get_by_local_and_remote_ap(AddressPort* local, AddressPort* remote) {
        mutex_.wait();
        if (how_sorted_ != AP) {
            socket_sort(ap_cmp);
            how_sorted_ = AP;
        }

        pair<AddressPort*,AddressPort*> p = make_pair(local, remote);
        Socket* value = binary_search(p, b_search_local_remote_cmp);
        mutex_.post();
        return value;
    }

    void push(Socket* s) {
        mutex_.wait();
        collection_.push_back(s);
        how_sorted_ = RE_SORT;
        mutex_.post();
    }

    int size() {
        return collection_.size();
    }

    int clear() {
        mutex_.wait();
        collection_.clear();
        mutex_.post();
    }

};



#endif	/* _SOCKETCOLLECTION_H */


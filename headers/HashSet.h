/* 
 * File:   NumberSet.h
 * Author: rbuck
 *
 * Created on December 22, 2010, 10:13 AM
 */

#ifndef _NUMBERSET_H
#define	_NUMBERSET_H

#include <tr1/unordered_set>

#include "Semaphore.h"

using namespace std;

template<class T>
class HashSet {
public:

    HashSet() {
        mutex_.init(1);
    }

    virtual ~HashSet() {

    }

    void insert(T obj) {
        mutex_.wait();
        try {
            set_.insert(obj);
        }        catch (...) {
            cout << "Error in insert" << endl;
            mutex_.post();
            return;
        }

        mutex_.post();
    }

    void remove(T obj) {
        mutex_.wait();
        set_.erase(obj);
        mutex_.post();
    }

    bool contains(T obj) {
        mutex_.wait();
        bool val = set_.find(obj) != set_.end();
        mutex_.post();
        return val;
    }

    int size() {
        mutex_.wait();
        int val = set_.size();
        mutex_.post();
        return val;
    }

    void clear() {
        mutex_.wait();
        set_.clear();
        mutex_.post();
    }

private:
    Semaphore mutex_;
    tr1::unordered_set<T> set_;
};

#endif	/* _NUMBERSET_H */


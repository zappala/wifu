/* 
 * File:   NumberSet.h
 * Author: rbuck
 *
 * Created on December 22, 2010, 10:13 AM
 */

#ifndef _NUMBERSET_H
#define	_NUMBERSET_H

#include <tr1/unordered_set>

using namespace std;

template<class T>
class HashSet {
public:
    HashSet() {

    }

    virtual ~HashSet() {

    }

    void insert(T obj) {

    }

    void remove(T obj) {

    }

    void contains(T obj) {

    }

    int size() {
        return set_.size();
    }

private:
    tr1::unordered_set<T> set_;
};

#endif	/* _NUMBERSET_H */


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

/**
 * Template hash set class that stores type T objects.
 * This class is thread-safe.
 */
template<class T>
class HashSet {
public:

    /**
     * Constructs an empty HashSet
     */
    HashSet() {
        reset();
    }

    /**
     * Cleans up this HashSet
     */
    virtual ~HashSet() {

    }

    /**
     * Inserts obj into this hash set
     * @param obj The object to insert
     */
    void insert(T obj) {
        mutex_->wait();
        set_.insert(obj);
        mutex_->post();
    }

    /**
     * Removes obj from this hash set if it exists
     */
    void remove(T obj) {
        mutex_->wait();
        set_.erase(obj);
        mutex_->post();
    }

    /**
     * @return True if obj is in this hash set, false otherwise
     */
    bool contains(T obj) {
        mutex_->wait();
        bool val = set_.find(obj) != set_.end();
        mutex_->post();
        return val;
    }

    /**
     * @return The number of elements in this hash set
     */
    int size() {
        mutex_->wait();
        int val = set_.size();
        mutex_->post();
        return val;
    }

    /**
     * Removes all elements from this hash set
     */
    void clear() {
        mutex_->wait();
        set_.clear();
        mutex_->post();
    }

    /**
     * Resets this hash set.
     * Reinitializes the protecting Semaphore object and calls clear()
     *
     * @see Semaphore
     * @see HashSet::clear()
     */
    void reset() {
        mutex_ = new Semaphore();
        mutex_->init(1);
        clear();
    }

private:
    Semaphore* mutex_;
    tr1::unordered_set<T> set_;
};

#endif	/* _NUMBERSET_H */


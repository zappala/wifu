/* 
 * File:   Set.h
 * Author: rbuck
 *
 * Created on December 22, 2010, 10:13 AM
 */

#ifndef _SET_H
#define	_SET_H

#include <set>
#include "Semaphore.h"
#include "GarbageCollector.h"

using namespace std;

/**
 * Template set class that stores type T objects.
 * This class is thread-safe.
 */
template<class T>
class Set {
public:

    /**
     * Constructs an empty Set
     */
    Set() {
        reset();
    }

    /**
     * Cleans up this Set
     */
    virtual ~Set() {

    }

    /**
     * Inserts obj into this set
     * @param obj The object to insert
     */
    void insert(T obj) {
        mutex_->wait();
        set_.insert(obj);
        mutex_->post();
    }

    /**
     * Removes obj from this set if it exists
     */
    void remove(T obj) {
        mutex_->wait();
        set_.erase(obj);
        mutex_->post();
    }

    /**
     * @return True if obj is in this set, false otherwise
     */
    bool contains(T obj) {
        mutex_->wait();
        bool val = set_.find(obj) != set_.end();
        mutex_->post();
        return val;
    }

    /**
     * @return The number of elements in this set
     */
    int size() {
        mutex_->wait();
        int val = set_.size();
        mutex_->post();
        return val;
    }

    /**
     * Removes all elements from this set
     */
    void clear() {
        mutex_->wait();
        set_.clear();
        mutex_->post();
    }

    /**
     * Resets this set.
     * Reinitializes the protecting Semaphore object and calls clear()
     *
     * @see Semaphore
     * @see Set::clear()
     */
    void reset() {
        mutex_ = new Semaphore();
        mutex_->init(1);
        clear();
    }

private:
    Semaphore* mutex_;
    set<T, std::less<T>, gc_allocator<T> > set_;
};

#endif	/* _NUMBERSET_H */


/* 
 * File:   ObjectPool.h
 * Author: rbuck
 *
 * Created on August 8, 2011, 11:29 AM
 */

#ifndef OBJECTPOOL_H
#define	OBJECTPOOL_H

#include <list>

#include "../applib/SocketData.h"
#include "defines.h"
#include "GarbageCollector.h"

template<class T>
class ObjectPool : public gc {
private:
    ObjectPool() : size_(0) {
        init();
    }

    void init() {
        mutex_.init(1);
        grow(POOL_INITIAL_SIZE);
    }

    void grow() {
        grow((size_ * 2) - size_);
    }

    void grow(int num) {
        for(int i = 0; i < num; ++i) {
            object_list_.push_back(new T());
            ++size_;
        }
    }

    Semaphore mutex_;
    list<T*, gc_allocator<T*> > object_list_;
    int size_;

public:

    virtual ~ObjectPool() {
        
    }

    static ObjectPool<T>& instance() {
        static ObjectPool<T> instance_;
        return instance_;
    }

    T* get() {
        mutex_.wait();
        if(object_list_.size() <= 0) {
            grow();
        }
        T* item = object_list_.front();
        object_list_.pop_front();
        mutex_.post();
        return item;
    }

    void release(T* item) {
        mutex_.wait();
        object_list_.push_back(item);
        mutex_.post();
    }

    int size() {
        mutex_.wait();
        int s = object_list_.size();
        mutex_.post();
        return s;
    }

    int capacity() {
        mutex_.wait();
        int c = size_;
        mutex_.post();
        return c;
    }
};

#endif	/* OBJECTPOOL_H */


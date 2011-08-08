/* 
 * File:   SocketDataPool.h
 * Author: rbuck
 *
 * Created on August 8, 2011, 11:29 AM
 */

#ifndef SOCKETDATAPOOL_H
#define	SOCKETDATAPOOL_H

#include <list>

#include "SocketData.h"
#include "defines.h"
#include "GarbageCollector.h"

class SocketDataPool : public gc {
private:
    SocketDataPool() : size_(0) {
        init();
    }

    void init() {
        mutex_.init(1);
        grow(SOCKET_DATA_POOL_INITIAL_SIZE);
        
    }

    void grow() {
        grow((size_ * 2) - size_);
    }

    void grow(int num) {
        for(int i = 0; i < num; ++i) {
            socket_data_list_.push_back(new SocketData());
            ++size_;
        }
    }

    Semaphore mutex_;
    list<SocketData*, gc_allocator<SocketData*> > socket_data_list_;
    int size_;

public:

    virtual ~SocketDataPool() {
        
    }

    static SocketDataPool& instance() {
        static SocketDataPool instance_;
        return instance_;
    }

    SocketData* get() {
        mutex_.wait();
        if(socket_data_list_.size() <= 0) {
            grow();
        }
        SocketData* item = socket_data_list_.front();
        socket_data_list_.pop_front();
        mutex_.post();
        return item;
    }

    void release(SocketData* item) {
        mutex_.wait();
        socket_data_list_.push_back(item);
        mutex_.post();
    }
};

#endif	/* SOCKETDATAPOOL_H */


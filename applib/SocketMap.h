/* 
 * File:   SocketMap.h
 * Author: rbuck
 *
 * Created on November 24, 2010, 11:51 AM
 */

#ifndef SOCKETMAP_H
#define	SOCKETMAP_H

#include <map>
#include "../headers/BinarySemaphore.h"

using namespace std;

class SocketMap {
private:

    SocketMap() {
        mutex_.init(1);
    }

    SocketMap(SocketMap const&) {

    }

    SocketMap & operator=(SocketMap const&) {

    }

    map<int, SocketData*> data_;
    BinarySemaphore mutex_;

public:

    virtual ~SocketMap() {
        map<int, SocketData*>::iterator itr;
        for (itr = data_.begin(); itr != data_.end(); ++itr) {
            if (itr->second != NULL) {
                delete itr->second;
            }
        }
    }

    static SocketMap& instance() {
        static SocketMap instance_;
        return instance_;
    }

    SocketData* get(const int key) {
        mutex_.wait();
        SocketData* ptr = data_[key];
        mutex_.post();
        return ptr;
    }

    void put(const int key, SocketData* value) {
        mutex_.wait();
        data_[key] = value;
        mutex_.post();
    }
};

#endif	/* SOCKETMAP_H */


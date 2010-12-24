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
#include "SocketData.h"

using namespace std;

class SocketDataMap {
private:

    SocketDataMap() {
        mutex_.init(1);
    }

    SocketDataMap(SocketDataMap const&) {

    }

    SocketDataMap & operator=(SocketDataMap const&) {

    }

    map<int, SocketData*> data_;
    BinarySemaphore mutex_;

public:

    virtual ~SocketDataMap() {
        map<int, SocketData*>::iterator itr;
        for (itr = data_.begin(); itr != data_.end(); ++itr) {
            if (itr->second != NULL) {
                delete itr->second;
            }
        }
    }

    static SocketDataMap& instance() {
        static SocketDataMap instance_;
        return instance_;
    }

    SocketData* get(const int key) {
        mutex_.wait();
        map<int, SocketData*>::iterator itr = data_.find(key);
        SocketData* ptr = itr == data_.end() ? NULL : itr->second;
        mutex_.post();
        return ptr;
    }

    void put(int key, SocketData* value) {

        mutex_.wait();
        data_[key] = value;
        mutex_.post();
    }

    void delete_at(const int key) {
        mutex_.wait();
        delete data_[key];
        data_.erase(key);
        mutex_.post();
    }

    void erase_at(const int key) {
        mutex_.wait();
        data_.erase(key);
        mutex_.post();
    }

    int size() {
        return data_.size();
    }
};

#endif	/* SOCKETMAP_H */


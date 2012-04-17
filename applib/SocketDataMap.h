/* 
 * File:   SocketMap.h
 * Author: rbuck
 *
 * Created on November 24, 2010, 11:51 AM
 */

#ifndef SOCKETMAP_H
#define	SOCKETMAP_H

#include <map>
#include <assert.h>
#include "Semaphore.h"
#include "SocketData.h"
#include "GarbageCollector.h"

using namespace std;

/**
 * Singleton map of socket id (int) to SocketData object.
 * This object is thread-safe.
 *
 * @see SocketData
 */
class SocketDataMap {
private:

    /**
     * Constructor.
     * Sets the internal mutex to 1.
     */
    SocketDataMap() {
        mutex_.init(1);
    }

    /**
     * Empty copy constructor.  Should never be called.
     * @param other The SocketDataMap to copy.
     */
    SocketDataMap(SocketDataMap const& other) {
        assert(false);
    }

    /**
     * Empty assignment operator.  Should never be called.
     * @param other The SocketDataMap to copy.
     */
    SocketDataMap & operator=(SocketDataMap const& other) {
        assert(false);
    }

    /**
     * Data structure mapping socket id (int) to SocketData pointer.
     */
    map<int, SocketData*, std::less<int>, gc_allocator<std::pair<int, SocketData*> > > data_;

    /**
     * Control variable that makes this object thread-safe.
     */
    Semaphore mutex_;

    /**
     * Iterator for data_.
     */
    map<int, SocketData*, std::less<int>, gc_allocator<std::pair<int, SocketData*> > >::iterator itr_;

public:

    /**
     * Destructor.
     * Because this class is a Singleton and the instance is static, this will be called upon exiting the program.
     * This calls delete on all SocketData pointers saved inside this data structure.
     */
    virtual ~SocketDataMap() {
        for (itr_ = data_.begin(); itr_ != data_.end(); ++itr_) {
            if (itr_->second != NULL) {
                delete itr_->second;
            }
        }
    }

    /**
     * @return The static instance of this SocketDataMap.
     */
    static SocketDataMap& instance() {
        static SocketDataMap instance_;
        return instance_;
    }

    /**
     * Gets the value (SocketData pointer) associated with key.
     * If there is no value associated with key, NULL is returned.
     *
     * @param key The socket id of the SocketData object needed.
     * @return The value (SocketData pointer) associated with key OR NULL if there is no association.
     */
    SocketData* get(const int key) {
        mutex_.wait();
        itr_ = data_.find(key);
        SocketData* ptr = itr_ == data_.end() ? NULL : itr_->second;
        mutex_.post();
        return ptr;
    }

    /**
     * Associates key with value.
     * If key already exists, the value is is associated with is overwriten with value.
     *
     * @param key The socket id of value.
     * @param value Pointer to a SocketData object to associate with key.
     */
    void put(int key, SocketData* value) {
        mutex_.wait();
        data_[key] = value;
        mutex_.post();
    }

    /**
     * Removes the key-value pair from the data structure.
     * Calls delete on the value.
     *
     * @param key The socket id of the SocketData object to remove and delete.
     */
    void delete_at(const int key) {
        mutex_.wait();
        delete data_[key];
        data_.erase(key);
        mutex_.post();
    }

    /**
     * Removes the key-value pair from the data structure, but does not delete the SocketData object.
     *
     * @param key The socket id of the SocketData object to remove.
     */
    void erase_at(const int key) {
        mutex_.wait();
        data_.erase(key);
        mutex_.post();
    }

    /**
     * @return The number of key-value pairs.
     */
    int size() {
        mutex_.wait();
        int s = data_.size();
        mutex_.post();
        return s;
    }
};

#endif	/* SOCKETMAP_H */


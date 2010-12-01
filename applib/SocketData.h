/* 
 * File:   SocketData.h
 * Author: rbuck
 *
 * Created on November 23, 2010, 2:37 PM
 */

#ifndef _SOCKETDATA_H
#define	_SOCKETDATA_H

#include <string.h>

#include "../headers/Semaphore.h"
#include "../headers/defines.h"

class SocketData {
public:
    SocketData() {
        sem_ = new Semaphore();
        sem_->init(0);
    }

    virtual ~SocketData() {
        delete sem_;
    }

    Semaphore* get_semaphore() {
        return sem_;
    }

    int& get_return_value() {
        return return_value_;
    }

    void set_return_value(int new_value) {
        this->return_value_ = new_value;
    }

    void set_payload(string& payload) {
        memcpy(payload_, payload.c_str(), payload.size());
    }

    void set_payload_length(int length) {
        payload_length_ = length;
    }

    unsigned char* get_payload() {
        return payload_;
    }

    int get_payload_length() {
        return payload_length_;
    }

private:
    int return_value_;
    Semaphore* sem_;
    unsigned char payload_[PAYLOAD_SIZE + 1];
    int payload_length_;
};

#endif	/* _SOCKETDATA_H */


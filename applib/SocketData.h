/* 
 * File:   SocketData.h
 * Author: rbuck
 *
 * Created on November 23, 2010, 2:37 PM
 */

#ifndef _SOCKETDATA_H
#define	_SOCKETDATA_H

#include "../headers/Semaphore.h"

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

private:
    int return_value_;
    Semaphore* sem_;
};

#endif	/* _SOCKETDATA_H */


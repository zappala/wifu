/* 
 * File:   LocalSocketReceiverThreads.h
 * Author: rbuck
 *
 * Created on December 3, 2010, 10:40 AM
 */

#ifndef _LOCALSOCKETRECEIVERTHREADS_H
#define	_LOCALSOCKETRECEIVERTHREADS_H

#include <vector>
#include "Semaphore.h"

using namespace std;

class LocalSocketReceiverThreadsSockets {
public:

    void push(pair<pthread_t, int> item) {
        sem_.wait();
        cout << "Pushing item: Thread: " << item.first << " Socket: " << item.second << endl;
        items_.push_back(item);
        sem_.post();
    }

    pair<pthread_t, int> pop() {
        sem_.wait();
        pair<pthread_t, int> thread = items_.back();
        items_.pop_back();
        sem_.post();
        return thread;
    }

    bool is_empty() {
        sem_.wait();
        bool empty = items_.empty();
        sem_.post();
        return empty;
    }


    LocalSocketReceiverThreadsSockets() {
        sem_.init(1);
    }

    ~LocalSocketReceiverThreadsSockets() {
        
    }

private:

    vector< pair<pthread_t, int> > items_;
    Semaphore sem_;
};

#endif	/* _LOCALSOCKETRECEIVERTHREADS_H */


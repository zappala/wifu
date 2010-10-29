/* 
 * File:   IQueue.h
 * Author: rbuck
 *
 * Created on October 27, 2010, 1:01 PM
 */

#ifndef _IQUEUE_H
#define	_IQUEUE_H

template<class T>
class IQueue {
public:
    IQueue() {}
    virtual ~IQueue() {}
    
    virtual T dequeue() = 0;
    virtual void enqueue(T obj, bool signal = false) = 0;
    virtual int size() = 0;
    virtual bool isEmpty() = 0;
};

#endif	/* _IQUEUE_H */


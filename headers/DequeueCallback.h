/* 
 * File:   DequeueCallback.h
 * Author: rbuck
 *
 * Created on October 27, 2010, 11:19 AM
 */

#ifndef _DEQUEUECALLBACK_H
#define	_DEQUEUECALLBACK_H

template<class T>
class DequeueCallback {
public:

    /**
     * Callback method called when the thread in QueueProcessor dequeues a T.
     *
     * @param object Object dequeued in QueueProcessor's dequeue thread.
     */
    virtual void process(T object) = 0;
};

#endif	/* _DEQUEUECALLBACK_H */


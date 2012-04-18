/* 
 * File:   BackLog.h
 * Author: rbuck
 *
 * Created on January 5, 2012, 9:26 AM
 */

#ifndef BACKLOG_H
#define	BACKLOG_H

#include "GarbageCollector.h"
#include "events/protocol_events/ConnectionEstablishedEvent.h"
#include <queue>

using namespace std;

class BackLog : public gc {
public:

    /**
     * Constructs a new BackLog
     * @param num_connections The maximum number of connection to hold on to
     */
    BackLog(int num_connections);

    /**
     * Destructor
     */
    ~BackLog();

    /**
     * Push e into the queue
     * @param e An event indicating a completed connection
     * @return True if the event was pushed; false indicates that pushing e would result in the size exceeding the capacity
     */
    bool push(ConnectionEstablishedEvent* e);

    /**
     * @return The next connection to return
     */
    ConnectionEstablishedEvent* pop();

    /**
     * @return The number of connections
     */
    int size() const;

    /**
     * @return The maximum number of connections supported
     */
    int capacity() const;

    /**
     * @return True of the underlying queue is empty, false otherwise
     */
    bool empty() const;
    

private:
    /**
     * Number of connections
     */
    int num_connections_;

    /**
     * Container holding the connections
     */
    queue<ConnectionEstablishedEvent*, deque<ConnectionEstablishedEvent*, gc_allocator<ConnectionEstablishedEvent*> > > back_log_;
    
};

#endif	/* BACKLOG_H */


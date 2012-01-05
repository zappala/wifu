/* 
 * File:   BackLog.h
 * Author: rbuck
 *
 * Created on January 5, 2012, 9:26 AM
 */

#ifndef BACKLOG_H
#define	BACKLOG_H

#include "GarbageCollector.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"
#include <queue>

using namespace std;

class BackLog : public gc {
public:

    /**
     * Constructs a new BackLog
     * @param num_connections The maximum number of connection requests to hold on to
     */
    BackLog(int num_connections);

    /**
     * Destructor
     */
    ~BackLog();

    /**
     * Push e into the queue
     * @param e An event containing a SYN segment
     * @return True if the event was pushed; false indicates that pushing e would result in the size exceeding the capacity
     */
    bool push(NetworkReceivePacketEvent* e);

    /**
     * @return The next SYN packet to process
     */
    NetworkReceivePacketEvent* pop();

    /**
     * @return The number of connections pending
     */
    int size() const;

    /**
     * @return The maximum number of connections supported
     */
    int capacity() const;
    

private:
    /**
     * Number of connections
     */
    int num_connections_;

    /**
     * Container holding the conneciton request packets
     */
    queue<NetworkReceivePacketEvent*> back_log_;
    
};

#endif	/* BACKLOG_H */


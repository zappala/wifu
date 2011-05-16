/* 
 * File:   TCPSequenceNumberComparator.h
 * Author: rbuck
 *
 * Created on May 16, 2011, 10:03 AM
 */

#ifndef TCPSEQUENCENUMBERCOMPARATOR_H
#define	TCPSEQUENCENUMBERCOMPARATOR_H

#include "TCPPacket.h"
#include "Math.h"

/**
 * When used with a PriorityQueue, this class will ensure that the TCPPacket pointers
 * are prioritized based on ascending sequence number
 */
class TCPSequenceNumberComparator {
    public:
    /**
     * Compares two TCPPacket pointers according to sequence number
     * Will return less_than(p2, p1)
     * 
     * @see less_than()
     * @return True if less_than(p2, p1)
     */
    bool operator()(TCPPacket*& p1, TCPPacket*& p2);
};

#endif	/* TCPSEQUENCENUMBERCOMPARATOR_H */


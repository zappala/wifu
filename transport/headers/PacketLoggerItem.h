/* 
 * File:   PacketLoggerItem.h
 * Author: rbuck
 *
 * Created on June 27, 2011, 10:20 AM
 */

#ifndef PACKETLOGGERITEM_H
#define	PACKETLOGGERITEM_H

#include "GarbageCollector.h"
#include "packet/WiFuPacket.h"
#include <sys/time.h>


class PacketLoggerItem : public gc {
private:
    WiFuPacket* packet_;
    struct timeval time_;

public:
    PacketLoggerItem(WiFuPacket* p);
    virtual ~PacketLoggerItem();

    WiFuPacket* get_packet() const;
    struct timeval* get_time();

};

#endif	/* PACKETLOGGERITEM_H */


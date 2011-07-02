/*
 * TCP_ATP.h
 *
 *  Created on: Jun 22, 2011
 *      Author: philipbl
 */

#ifndef TCP_ATP_H_
#define TCP_ATP_H_

#include "contexts/ATPIContextContainer.h"
#include "IContextContainerFactory.h"
#include "ATPIContextContainerFactory.h"
#include "contexts/BasicIContextContainer.h"

#include "TCPTahoe.h"

class TCPATP: public TCPTahoe {
private:
    IContextContainerFactory* factory_;

protected:
    TCPATP(int protocol = TCP_ATP, IContextContainerFactory* factory = new ATPIContextContainerFactory());

public:
	virtual ~TCPATP();

	static TCPATP& instance();

    virtual void icontext_receive_packet(QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);

    virtual void icontext_send_packet(QueueProcessor<Event*>* q, SendPacketEvent* e);

};

#endif /* TCP_ATP_H_ */

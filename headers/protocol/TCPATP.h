/*
 * TCP_ATP.h
 *
 *  Created on: Jun 22, 2011
 *      Author: philipbl
 */

#ifndef TCP_ATP_H_
#define TCP_ATP_H_

#include "contexts/ATPIContextContainer.h"


#include "TCPTahoe.h"

class TCPATP: public TCPTahoe {

protected:
    TCPATP(int protocol = TCP_ATP);

public:
	virtual ~TCPATP();

	static TCPATP& instance();

    virtual void icontext_socket(QueueProcessor<Event*>* q, SocketEvent* e);

    virtual void icontext_receive_packet(QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);

    virtual void icontext_send_packet(QueueProcessor<Event*>* q, SendPacketEvent* e);

    virtual void icontext_new_connection_initiated(QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e);

};

#endif /* TCP_ATP_H_ */

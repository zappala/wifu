/* 
 * File:   DummyProtocol.h
 * Author: rbuck
 *
 * Created on July 19, 2011, 11:41 AM
 */

#ifndef DUMMYPROTOCOL_H
#define	DUMMYPROTOCOL_H

// we will use Tahoe as a base so we can utilize the connection manager.
// in reality tahoe should inherit from dummy and dummy should provide the connection stuff
#include "TCPTahoe.h"
#include "defines.h"
#include "DummyIContextContainerFactory.h"

// Ten million bytes
#define DUMMY_BUFFER_SIZE 10000000

using namespace std;

class DummyProtocol : public TCPTahoe {
protected:
    DummyProtocol(int protocol = DUMMY_PROTO, IContextContainerFactory* factory = new DummyIContextContainerFactory());

public:
    virtual ~DummyProtocol();
    static DummyProtocol& instance();

    // Need these two to set the socket buffer sizes to be really big
    virtual void icontext_socket(QueueProcessor<Event*>* q, SocketEvent* e);
    virtual void icontext_new_connection_initiated(QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e);

    virtual void icontext_send(QueueProcessor<Event*>* q, SendEvent* e);
    virtual void icontext_receive(QueueProcessor<Event*>* q, ReceiveEvent* e);

private:
    void set_socket_buffer_sizes(Socket* s, int size);
    void dispatch_received_data(ReceiveEvent* e);
    
};

#endif	/* DUMMYPROTOCOL_H */


/*
 * @file Receiver.h
 *
 * @date Jun 17, 2011
 * @author philipbl
 */

#ifndef RECEIVEDATA_H_
#define RECEIVEDATA_H_

#include "TCPATPCongestionControl.h"
#include "contexts/ATPCongestionControlContext.h"

/**
 * The state that is used when receiving data (not ACKs) from a sender.
 * It reads the max delay from each packet received and calculates the
 * new average delay. This new average delay is then inserted back into
 * the packet.
 *
 * @see TCPATPCongestionControl
 * @see Waiting
 * @see Sender
 */
class Receiver: public TCPATPCongestionControl {
public:
	/**
	 * Constructor
	 */
	Receiver();

	/**
	 * Destructor
	 */
	virtual ~Receiver();

	/**
	 * This is used when transitioning from Waiting to Receiver.
	 * It is never called within Receiver itself.
	 * Gets the max delay from the context and sets it to the average delay
	 * (see 4.3.1).
	 *
	 * @param c An instance of ATPCongestionControlContext
	 */
    virtual void state_enter(Context* c);

    /**
     * Handles a request by Context::icontext_receive_packet() when a NetworkReceivePacketEvent occurs.
     * Reads max delay from packet and updates the average delay.
     *
	 * @param c The Context containing data and which called this method.
	 * @param q Pointer to the QueueProcessor which dequeued e.
	 * @param e The NetworkReceivePacketEvent which caused this method to (eventually) be called.
     */
    virtual void state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);

    /**
     * Handles a request by Context::icontext_send() when a SendPacketEvent occurs.
     * The packet is checked to make sure it is an ACK. The average delay is then
     * set on the packet and the packet is sent immediately.
     *
     * @param c The Context containing data and which called this method.
	 * @param q Pointer to the QueueProcessor which dequeued e.
	 * @param e The NetworkReceivePacketEvent which caused this method to (eventually) be called.
     */
    virtual void state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e);

    /**
     * Handles a request by Context::icontext_send_buffer_not_empty() when a SendBufferNotEmptyEvent occurs.
     * Switches to Sender state and calls state_send_buffer_not_emptpy on the Sender.
     *
     * @param c The Context containing data and which called this method.
	 * @param q Pointer to the QueueProcessor which dequeued e.
	 * @param e The NetworkReceivePacketEvent which caused this method to (eventually) be called.
     */
    virtual void state_send_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e);

    virtual void state_receive_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent* e);

    virtual void state_receive_buffer_not_full(Context* c, QueueProcessor<Event*>* q, ReceiveBufferNotFullEvent* e);


private:
    /**
     * Calculates the delay based on 4.2.1.
     *
     * @param max the max delay read from packet (in milliseconds)
     * @param previous the average delay (in milliseconds)
     * @param beta how much weight the current delay has
     */
    u_int32_t calculateDelay(u_int32_t max, u_int32_t previous, double beta);

};

#endif /* RECEIVEDATA_H_ */

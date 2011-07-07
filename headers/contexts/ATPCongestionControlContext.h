/**
 * @file ATPCongestionControlContext.h
 *
 * @date Jun 16, 2011
 * @author philipbl
 */

#ifndef ATPCONGESTIONCONTROLCONTEXT_H_
#define ATPCONGESTIONCONTROLCONTEXT_H_

#include "TCPTahoeCongestionControlContext.h"
#include "states/atp/Waiting.h"
#include "defines.h"


/**
 * Keeps track of the state and delay for TCPATP's congestion control.
 * The delay is used to figure out what the sending rate should be.
 * It also keeps track of the time between reading events (see ATP Paper 4.2.3)
 *
 * @see TCPATP
 * @see TCPTahoeCongestionControlContext
 * @see TCPATPCongestionControl
 * @see Waiting
 * @see Sender
 * @see Receiver
 *
 */
class ATPCongestionControlContext : public TCPTahoeCongestionControlContext {
public:
	/**
	 * Constructor.
	 * Sets the initial state to Waiting.
	 *
	 * @param iss Initial send sequence number (used in TCPTahoeCongestionControlContext)
	 *
	 * @see TCPTahoeCongestionControlContext
	 */
	ATPCongestionControlContext(u_int32_t iss);

	/**
	 * Destructor.
	 */
	virtual ~ATPCongestionControlContext();

	/**
	 * Stores the weighted average of all the packets that have been received.
	 * (see 4.2.1).
	 *
	 * @return the average delay (in microseconds)
	 */
	u_int32_t get_average_delay() const;

	/**
	 * After a new packet is received, the max delay is read from that packet and
	 * used to calculate the new average delay (see 4.2.1).
	 *
	 * @param d the new average after a new packet has been received (in microseconds)
	 */
	void set_average_delay(u_int32_t d);

	/**
	 * Stores the max delay of a packet.
	 * Only used when transitioning from Waiting state to Receive state,
	 * otherwise the max delay is read directly from the packet.
	 *
	 * @return the max delay of a received packet (in microseconds)
	 */
	u_int32_t get_max_delay() const;

	/**
	 * Sets the max delay read from a packet.
	 * Only used when transitioning from Waiting state to Receive state.
	 *
	 * @param d the max delay read from packet (in microseconds)
	 */
	void set_max_delay(u_int32_t d);

	/**
	 * Returns the delay between reading events (see 4.2.3)
	 *
	 * @return the delay between reading events (in microseconds)
	 */
	u_int32_t get_reading_delay();

	/**
	 * Sets the delay between reading events (see 4.2.3)
	 *
	 * @param d the delay between reading events (in microseconds)
	 */
	void set_reading_delay(u_int32_t d);

	void start_timer();

	u_int32_t stop_timer();

	void set_receive_buffer_size(int size);

	int get_receive_buffer_size();


private:
	/**
	 * Holds the averaged delay (in microseconds) of all incoming packets.
	 * Every time a new packet comes in this value and the max delay are used
	 *  to calculate the new average delay (see 4.2.1).
	 */
	u_int32_t average_delay_;

	/**
	 * Holds the max delay (in microseconds) of the most recent received packet.
	 */
	u_int32_t max_delay_;

	/**
	 * Holds the time difference between reading events (in microseconds).
	 * (see 4.2.3)
	 */
	u_int32_t read_delay_;

	/**
	 * Used to calculate the time between reading events
	 */
	struct timespec start_;

	int receive_buffer_size_;
};


#endif /* ATPCONGESTIONCONTROLCONTEXT_H_ */

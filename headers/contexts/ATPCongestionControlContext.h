/*
 * ATPCongestionControlContext.h
 *
 *  Created on: Jun 16, 2011
 *      Author: philipbl
 */

#ifndef ATPCONGESTIONCONTROLCONTEXT_H_
#define ATPCONGESTIONCONTROLCONTEXT_H_

#include "TCPTahoeCongestionControlContext.h"
#include "states/atp/Waiting.h"
#include "defines.h"

class ATPCongestionControlContext : public TCPTahoeCongestionControlContext {
public:
	ATPCongestionControlContext(u_int32_t iss);
	virtual ~ATPCongestionControlContext();

	u_int32_t get_average_delay() const;

	void set_average_delay(u_int32_t d);

	u_int32_t get_max_delay() const;

	void set_max_delay(u_int32_t d);

	void set_reading_delay(u_int32_t d);

	u_int32_t get_reading_delay();

	u_int32_t get_time_difference();





private:

	u_int32_t average_delay_;
	u_int32_t max_delay_;

	u_int32_t read_delay_;
	struct timespec start_;
};


#endif /* ATPCONGESTIONCONTROLCONTEXT_H_ */

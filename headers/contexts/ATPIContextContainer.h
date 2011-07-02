/*
 * ATPIContextContainer.h
 *
 *  Created on: Jun 16, 2011
 *      Author: philipbl
 */

#ifndef ATPICONTEXTCONTAINER_H_
#define ATPICONTEXTCONTAINER_H_

#include "TCPTahoeIContextContainer.h"
#include "ATPCongestionControlContext.h"

class ATPIContextContainer : public TCPTahoeIContextContainer {
public:
	ATPIContextContainer();
	virtual ~ATPIContextContainer();


private:
    IContext* congestion_control_;

};

#endif /* ATPICONTEXTCONTAINER_H_ */

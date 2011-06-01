/*
 * File:   RateLimiterContext.h
 * Author: leer
 *
 */

#ifndef RATELIMITERCONTEXT_H
#define	RATELIMITERCONTEXT_H

#include <string>
#include "Context.h"
//#include "states/PassThroughRateLimiter.h"
#include "states/tcp-ap/SendRateLimiter.h"

using namespace std;

class RateLimiterContext : public Context {
public:
    RateLimiterContext();

};

#endif	/* RATELIMITERCONTEXT_H */

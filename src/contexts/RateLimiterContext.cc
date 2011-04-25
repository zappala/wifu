#include "contexts/RateLimiterContext.h"

RateLimiterContext::RateLimiterContext() : Context(){
    set_state(new PassThroughRateLimiter());
}

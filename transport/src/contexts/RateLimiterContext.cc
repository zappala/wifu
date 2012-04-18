#include "contexts/RateLimiterContext.h"

/*RateLimiterContext::RateLimiterContext() : Context(){
    set_state(new PassThroughRateLimiter());
}*/

RateLimiterContext::RateLimiterContext() : Context(){
    set_state(new SendRateLimiter());
    SendRateLimiter* state = (SendRateLimiter*)get_state();
    //Seconds, nanoseconds; 0,0 = pass-through?
    state->setRate(0,0);
}
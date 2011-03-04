#include "contexts/CongestionControlContext.h"

CongestionControlContext::CongestionControlContext() : Context() {
    set_state(new SlowStart());
}

CongestionControlContext::~CongestionControlContext() {

}

#include "BackLog.h"

BackLog::BackLog(int num_connections) : num_connections_(num_connections) {

}

BackLog::~BackLog() {

}

bool BackLog::push(NetworkReceivePacketEvent* e) {
    if(size() + 1 <= capacity()) {
        back_log_.push(e);
        return true;
    }
    return false;
}

NetworkReceivePacketEvent* BackLog::pop() {
    NetworkReceivePacketEvent* e = back_log_.front();
    back_log_.pop();
    return e;
}

int BackLog::size() const {
    return back_log_.size();
}

int BackLog::capacity() const {
    return num_connections_;
}

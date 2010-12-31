#include "../headers/states/SlowStart.h"

SlowStart::SlowStart() {

}

SlowStart::~SlowStart() {

}

void SlowStart::receive(Context* c, string& data) {

}

void SlowStart::enter(Context* c) {
    enter_state("SlowStart");
}

void SlowStart::exit(Context* c) {
    leave_state("SlowStart");
}

void SlowStart::connect(Context* c, string& remote) {
    //cout << "SlowStart Connecting to: " << remote << endl;
}


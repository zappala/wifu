/* 
 * File:   SimpleUDPIContextContainer.cc
 * Author: leer
 * 
 * Created on June 21, 2011, 9:57 AM
 */

#include "contexts/SimpleUDPIContextContainer.h"

SimpleUDPIContextContainer::SimpleUDPIContextContainer() : saved_send_event_(0), saved_close_event_(0), fin_(0) {
    
    //connection_manager_ = new ConnectionManagerContext();
    //congestion_control_ = new SimpleUDPCongestionControlContext(iss);
    reliability_ = new SimpleUDPReliabilityContext();
}

SimpleUDPIContextContainer::~SimpleUDPIContextContainer() {

}

IContext* SimpleUDPIContextContainer::get_connection_manager() {
    return 0;
}

IContext* SimpleUDPIContextContainer::get_congestion_control() {
    return 0;
}

IContext* SimpleUDPIContextContainer::get_reliability() {
    return reliability_;
}

SendEvent* SimpleUDPIContextContainer::get_saved_send_event() {
    return 0;
}

void SimpleUDPIContextContainer::set_saved_send_event(SendEvent* e) {
    //saved_send_event_ = e;
}

CloseEvent* SimpleUDPIContextContainer::get_saved_close_event() {
    return 0;
}

void SimpleUDPIContextContainer::set_saved_close_event(CloseEvent* e) {
    //saved_close_event_ = e;
}

NetworkReceivePacketEvent* SimpleUDPIContextContainer::get_saved_fin() {
    return 0;
}

void SimpleUDPIContextContainer::set_saved_fin(NetworkReceivePacketEvent* e) {
    //fin_ = e;
}


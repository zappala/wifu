#include "MockPortManager.h"
#include "OptionParser.h"

MockPortManager::MockPortManager() : IPortManager(), RandomNumberSet<u_int16_t>() {}

MockPortManager::~MockPortManager() {}

MockPortManager& MockPortManager::instance() {
	static MockPortManager instance_;
	return instance_;
}

u_int16_t MockPortManager::get() {
    gcstring s = "passive_port";
    gcstring val = "0";
    if(OptionParser::instance().present(s)) {
        val = OptionParser::instance().argument(s);
    } 

    return atoi(val.c_str());

}

void MockPortManager::remove(u_int16_t port) {

}

void MockPortManager::reset() {

}

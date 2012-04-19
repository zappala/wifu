#include "handler.h"

Handler::Handler() {
}

Handler::~Handler() {
}

void Handler::parse_options(int argc, char* const argv[]) {
}

void Handler::input(IPPacket& p) {
    p.accept();
}

void Handler::output(IPPacket& p) {
    p.accept();
}

void Handler::forward(IPPacket& p) {
    p.accept();
}

void Handler::prerouting(IPPacket& p) {
    p.accept();
}

void Handler::postrouting(IPPacket& p) {
    p.accept();
}

void Handler::start() {
}

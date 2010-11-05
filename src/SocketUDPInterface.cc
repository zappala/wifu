#include "SocketUDPInterface.h"
#include "SocketDispatcher.h"


SocketUDPInterface * SocketUDPInterface::instance_ = NULL;

// private

SocketUDPInterface::SocketUDPInterface(string & file) : LocalSocketFullDuplex(file) {


}

SocketUDPInterface::SocketUDPInterface(SocketUDPInterface const&) : LocalSocketFullDuplex(getFile()) {

}

SocketUDPInterface & SocketUDPInterface::operator=(SocketUDPInterface const&) {

}

// public

SocketUDPInterface::~SocketUDPInterface() {

}

SocketUDPInterface * SocketUDPInterface::instance() {
    if (!instance_) {
        string filename("/tmp/udpinterface");
        instance_ = new SocketUDPInterface(filename);
    }
    return instance_;
}

void SocketUDPInterface::receive(string & message) {
    cout << "UDPInterface Receive: " << message << endl;
    if(message.find("<connect>", 0) != string::npos) {
        sleep(2);
        string reply("<connected> localhost");
        send_to(SocketDispatcher::instance().getFile(),reply);
    }
}

void SocketUDPInterface::destroy(void) {
    delete instance();
}


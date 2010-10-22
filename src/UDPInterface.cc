#include "UDPInterface.h"
#include "Dispatcher.h"


UDPInterface * UDPInterface::instance_ = NULL;

// private

UDPInterface::UDPInterface(string & file) : LocalSocketFullDuplex(file) {


}

UDPInterface::UDPInterface(UDPInterface const&) : LocalSocketFullDuplex(getFile()) {

}

UDPInterface & UDPInterface::operator=(UDPInterface const&) {

}

// public

UDPInterface::~UDPInterface() {

}

UDPInterface * UDPInterface::instance() {
    if (!instance_) {
        string filename("/tmp/udpinterface");
        instance_ = new UDPInterface(filename);
    }
    return instance_;
}

void UDPInterface::receive(string & message) {
    cout << "UDPInterface Receive: " << message << endl;
    if(message.find("<connect>", 0) != string::npos) {
        sleep(2);
        string reply("<connected> localhost");
        send_to(Dispatcher::instance()->getFile(),reply);
    }
}

void UDPInterface::destroy(void) {
    delete instance();
}


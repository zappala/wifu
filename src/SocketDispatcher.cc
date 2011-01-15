#include "SocketDispatcher.h"

// private
SocketDispatcher::SocketDispatcher(string & file) : LocalSocketFullDuplex(file) {
    string connection_manager("/tmp/connection_manager_file");
    modules_.push_back(new SocketConnectionManager(connection_manager));
    
    string reliability("/tmp/reliability_file");
    modules_.push_back(new SocketReliability(reliability));

    string congestion_control("/tmp/congestion_control");
    modules_.push_back(new SocketCongestionControl(congestion_control));

}

SocketDispatcher::SocketDispatcher(SocketDispatcher const&) : LocalSocketFullDuplex(getFile()){

}

SocketDispatcher & SocketDispatcher::operator=(SocketDispatcher const&) {

}

// public
SocketDispatcher::~SocketDispatcher() {

    while(!modules_.empty()) {
        ISocketModule * module = modules_.back();
        modules_.pop_back();
        delete module;
    }
}

SocketDispatcher & SocketDispatcher::instance() {
    string filename("/tmp/dispatcher");
    static SocketDispatcher instance_(filename);
    return instance_;
}

void SocketDispatcher::receive(string & message) {

    for(int i = 0; i < modules_.size(); i++) {

        if(message.find("<connect>", 0) != string::npos) {
            modules_.at(i)->connect(message);
        }
        else if(message.find("<data>", 0) != string::npos) {
            modules_.at(i)->data(message);
        }
    }
}

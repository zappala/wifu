#include "Dispatcher.h"

Dispatcher * Dispatcher::instance_ = NULL;

// private
Dispatcher::Dispatcher(string & file) : LocalSocketFullDuplex(file) {
    string connection_manager("/tmp/connection_manager_file");
    modules_.push_back(new ConnectionManager(connection_manager));
    
    string reliability("/tmp/reliability_file");
    modules_.push_back(new Reliability(reliability));

    string congestion_control("/tmp/congestion_control");
    modules_.push_back(new CongestionControl(congestion_control));

}

Dispatcher::Dispatcher(Dispatcher const&) : LocalSocketFullDuplex(getFile()){

}

Dispatcher & Dispatcher::operator=(Dispatcher const&) {

}

// public
Dispatcher::~Dispatcher() {

    while(!modules_.empty()) {
        IModule * module = modules_.back();
        modules_.pop_back();
        delete module;
    }
}

Dispatcher * Dispatcher::instance() {
    if(!instance_) {
        string filename("/tmp/dispatcher");
        instance_ = new Dispatcher(filename);
    }
    return instance_;
}

void Dispatcher::receive(string & message) {
    cout << "Dispatcher Receive: " << message << endl;

    for(int i = 0; i < modules_.size(); i++) {

        if(message.find("<connect>", 0) != string::npos) {
            modules_.at(i)->connect(message);
        }
        else if(message.find("<data>", 0) != string::npos) {
            modules_.at(i)->data(message);
        }
    }
}

void Dispatcher::destroy(void) {
    delete instance();
    instance_ = NULL;
}

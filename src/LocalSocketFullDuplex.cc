#include "LocalSocketFullDuplex.h"

LocalSocketFullDuplex::LocalSocketFullDuplex(string & file) : LocalSocketSender(), LocalSocketReceiver(file, this), LocalSocketReceiverCallback() {
}

LocalSocketFullDuplex::LocalSocketFullDuplex(const char* file) : LocalSocketSender(), LocalSocketReceiver(file, this), LocalSocketReceiverCallback() {

}

LocalSocketFullDuplex::~LocalSocketFullDuplex() {

}
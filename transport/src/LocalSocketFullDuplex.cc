#include "LocalSocketFullDuplex.h"

LocalSocketFullDuplex::LocalSocketFullDuplex(gcstring& file) : LocalSocketSender(), LocalSocketReceiver(file, this), LocalSocketReceiverCallback() {
}

LocalSocketFullDuplex::LocalSocketFullDuplex(const char* file) : LocalSocketSender(), LocalSocketReceiver(file, this), LocalSocketReceiverCallback() {

}

LocalSocketFullDuplex::~LocalSocketFullDuplex() {

}
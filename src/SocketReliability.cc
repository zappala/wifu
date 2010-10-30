#include "SocketReliability.h"

SocketReliability::SocketReliability(string& file) : LocalSocketFullDuplex(file) {

}

SocketReliability::~SocketReliability() {

}

void SocketReliability::receive(string & message) {
    cout << "Reliability receive(): " << message << endl;
    cout << "Length of message: " << message.length() << endl;
}

void SocketReliability::connect(string & message) {
    cout << "Reliability connect: " << message << endl;
}

void SocketReliability::data(string & message) {
    cout << "Reliability data: " << message << endl;
}
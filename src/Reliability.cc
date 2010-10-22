#include "Reliability.h"

Reliability::Reliability(string& file) : LocalSocketFullDuplex(file) {

}

Reliability::~Reliability() {

}

void Reliability::receive(string & message) {
    cout << "Reliability receive(): " << message << endl;
    cout << "Length of message: " << message.length() << endl;
}

void Reliability::connect(string & message) {
    cout << "Reliability connect: " << message << endl;
}

void Reliability::data(string & message) {
    cout << "Reliability data: " << message << endl;
}
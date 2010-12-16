/* 
 * File:   SimpleTCP.cc
 * Author: rbuck
 *
 * Created on December 16, 2010, 1:40 PM
 */

#include <stdlib.h>
#include <iostream>

#include "../headers/UDPSocket.h"
#include "../headers/Identifiable.h"
#include "../headers/Queue.h"

#define MSS 1500

using namespace std;

class SimplePacket : public Identifiable {
public:

    SimplePacket(unsigned char* data, size_t length) : length_(length) {
        memcpy(buffer_ + sizeof (int), data, length);
        int id = get_id();
        memcpy(buffer_, &id, sizeof (int));
    }

    virtual ~SimplePacket() {

    }

    unsigned char* to_bytes() {
        return buffer_;
    }

    size_t length() {
        return length_ + sizeof (int);
    }

private:
    size_t length_;
    unsigned char buffer_[MSS];
};

class SimpleTCP : public UDPSocketCallback {
public:

    SimpleTCP(AddressPort& ap) : UDPSocketCallback() {
        socket_.bind_socket(ap);
        socket_.receive(this);

        local_ = 0;
        remote_ = 0;

        queue_flag_.init(0);
    }

    virtual ~SimpleTCP() {
        if (local_) {
            delete local_;
        }

        if (remote_) {
            delete remote_;
        }
    }

    void receive(AddressPort& ap, unsigned char* buffer, size_t length) {

    }

    void send(unsigned char* buffer, size_t length) {
        // fill up the Queue here,
    }

    void connect(AddressPort& ap) {
        
        //SimplePacket* p = new SimplePacket()
    }

private:
    UDPSocket socket_;

    // Local address and port that this object is bound to
    AddressPort* local_;

    // Remote address and port to which to send to
    AddressPort* remote_;

    // Queue to hold the packets
    Queue<SimplePacket*> queue_;

    Semaphore queue_flag_;

    

};

/*
 * 
 */
int main(int argc, char** argv) {

    cout << "SimpleTCP" << endl;
    return (EXIT_SUCCESS);
}


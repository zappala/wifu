/* 
 * File:   SocketData.h
 * Author: rbuck
 *
 * Created on November 23, 2010, 2:37 PM
 */

#ifndef _SOCKETDATA_H
#define	_SOCKETDATA_H

#include <string.h>

#include "Semaphore.h"
#include "defines.h"
#include "AddressPort.h"

/**
 * Object which stores responses from the back-end.
 * This object also holds the control mechanism for each respective socket.
 * There is a one-to-one relationship between socket ids (int) and SocketData objects.
 *
 * All internals of this object get set in the WifuEndAPILocalSocket::receive() method
 * and will be used in whatever socket API method was called.
 *
 * @see SocketDataMap
 */
class SocketData {
public:

    /**
     * Constructor
     * Initializes a new SocketData object
     * Sets the internal Semaphore object to 0
     *
     * @see Semaphore
     */
    SocketData() {
        sem_ = new Semaphore();
        sem_->init(0);

        flag_ = new Semaphore();
        flag_->init(1);
    }

    /**
     * Destructor
     */
    virtual ~SocketData() {
        delete sem_;
    }

    /**
     * @return A pointer to the internal Semaphore
     *
     * @see Semaphore
     */
    Semaphore* get_semaphore() {
        return sem_;
    }

    Semaphore* get_flag() {
        return flag_;
    }

    /**
     * @return the return value of the current socket API call
     */
    int& get_return_value() {
        return return_value_;
    }

    /**
     * Sets the return value of the current socket API call
     * This method will be called in WifuEndAPILocalSocket::receive()
     *
     * @param new_value The return value to save
     *
     * @see WifuEndAPILocalSocket::receive()
     */
    void set_return_value(int new_value) {
        this->return_value_ = new_value;
    }

    /**
     * Sets the payload returned from the back-end on any API call that needs the payload, for example wifu_receive()
     * This method will be called in WifuEndAPILocalSocket::receive()
     *
     * @param payload The payload to save
     *
     * @see WifuEndAPILocalSocket::receive()
     */
    void set_payload(string& payload) {
        memcpy(payload_, payload.c_str(), payload.size());
    }

    /**
     * Sets the length of the payload returned from the back-end on any API call that needs the payload, for example wifu_receive()
     * This method will be called in WifuEndAPILocalSocket::receive()
     *
     * @param length The length of payload
     *
     * @see WifuEndAPILocalSocket::receive()
     */
    void set_payload_length(int length) {
        payload_length_ = length;
    }

    /**
     * Gets the payload returned from the back-end
     * This could be called in wifu_receive()
     *
     * @return A pointer to the payload as returned by the back-end
     */
    unsigned char* get_payload() {
        return payload_;
    }

    /**
     * Gets the payload length returned from the back-end
     * This could be called in wifu_receive()
     *
     * @return The length of the payload as returned by the back-end
     */
    int get_payload_length() {
        return payload_length_;
    }

    /**
     * Stores the ERRNO value as returned by the back-end
     *
     * @param error The value ERRNO should be set
     */
    void set_error(int error) {
        this->error_ = error;
    }

    /**
     * @return The ERRNO value as returned by the back-end.
     */
    int get_error() {
        return error_;
    }

    /**
     * Stores the AddressPort as returned by the back-end.
     * This could possibly be used by accept() where the remote peer is needed.
     *
     * @param ap Pointer to the AddressPort to save.  This should have previously been new'd.
     */
    void set_address_port(AddressPort* ap) {
        address_ = ap;
    }

    /**
     * @return The AddressPort as returned by the back-end.
     */
    AddressPort* get_address_port() {
        return address_;
    }

    /**
     * @return The length of the sockaddr object internal to the AddressPort object
     *
     * @see SocketData::get_address_port()
     */
    socklen_t get_address_port_length() {
        return sizeof(* get_address_port()->get_network_struct_ptr());
    }

private:
    /**
     * The return value of any method call in the wifu_socket API
     */
    int return_value_;

    /**
     * The value ERRNO should be set to upon returning from any call in the wifu_socket API
     */
    int error_;

    /**
     * The internal Semaphore used to control access and responses to wifu_socket API calls
     */
    Semaphore* sem_;

    /**
     * Buffer which will store the payload from the back-end (on calls like recv())
     */
    unsigned char payload_[MTU + 1];

    /**
     * The length of the payload
     */
    int payload_length_;

    /**
     * The AddressPort returned from the back-end (on calls like accept())
     */
    AddressPort* address_;

    Semaphore* flag_;

    

};

#endif	/* _SOCKETDATA_H */


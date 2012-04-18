/* 
 * File:   SocketData.h
 * Author: rbuck
 *
 * Created on November 23, 2010, 2:37 PM
 */

#ifndef _SOCKETDATA_H
#define	_SOCKETDATA_H

#include "Semaphore.h"
#include "defines.h"
#include "AddressPort.h"
#include "GarbageCollector.h"

/**
 * Object which stores responses from the back-end.
 * This object also holds the Semaphores to ensure a thread-safety mechanism for each respective socket.
 * There is a one-to-one relationship between socket ids (int) and SocketData objects.
 *
 * All internals of this object get set in the WiFuTransportFrontEndTranslator::receive() method
 * and will be used in whatever socket API method was called.
 *
 * @see SocketDataMap
 */
class SocketData : public gc {
public:

    /**
     * Constructor.
     * Initializes a new SocketData object.
     * Sets the internal Semaphore object to 0.
     * Sets the internal flag object to 1.
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
     * Destructor.
     */
    virtual ~SocketData() {

    }

    /**
     * @return A pointer to the internal Semaphore.  Used to provide thread-safety.
     *
     * @see Semaphore
     */
    Semaphore* get_semaphore() {
        return sem_;
    }

    /**
     * 
     * @return A pointer to the internal flag object.  Used to provide thread-safety.
     */
    Semaphore* get_flag() {
        return flag_;
    }

    /**
     * Copies length bytes from the location pointed to by response into an internal buffer.
     *
     * @param response Pointer to a message received from the back end.
     * @param length Number of bytes to copy.
     */
    void set_receive_payload(unsigned char* response, int length) {
        memcpy(receive_payload_, response, length);
    }

    /**
     * @return A pointer to the internal buffer that holds responses from the back end.
     */
    unsigned char* get_receive_payload() {
        return receive_payload_;
    }

    /**
     * This is a pointer to an internal buffer so a socket doesn't need to recreate a buffer each time it needs to communicate with the back end.
     * @return A pointer to the internal buffer used to insert the BSD socket API parameters into to send to the back end.
     *
     */
    unsigned char* get_send_payload() {
        return send_payload_;
    }

private:

    /**
     * Used to coordinate the sending and receiving of messages to and from the back end.
     * This is used in conjunction with flag_.
     */
    Semaphore* sem_;

    /**
     * Used to coordinate the sending and receiving of messages to and from the back end.
     * This is used in conjunction with sem_.
     */
    Semaphore* flag_;

    /**
     * Buffer which will store the payload from the back-end (on calls like recv()).
     */
    unsigned char receive_payload_[UNIX_SOCKET_MAX_BUFFER_SIZE];

    /**
     * Buffer which will store the data to be sent to the back end
     */
     unsigned char send_payload_[UNIX_SOCKET_MAX_BUFFER_SIZE];
    

};

#endif	/* _SOCKETDATA_H */


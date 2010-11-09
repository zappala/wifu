/* 
 * File:   AddressPort.h
 * Author: rbuck
 *
 * Created on October 30, 2010, 10:06 AM
 */

#ifndef _ADDRESSPORT_H
#define	_ADDRESSPORT_H

#include <string>

using namespace std;

/**
 * Represents an Address-Port combination.
 */
class AddressPort {
public:

    /**
     * Constructor: Stores address and port.
     *
     * @param address Address to store
     * @param port Port to store
     */
    AddressPort(string& address, string& port) : address_(address), port_(port) {

    }

    /**
     * Destructor
     */
    virtual ~AddressPort() {

    }

    /**
     * @return A reference to the address
     */
    string& get_address() {
        return address_;
    }

    /**
     * @return A reference to the port
     */
    string& get_port() {
        return port_;
    }

private:
    /**
     * The (IP) address of a machine.
     */
    string address_;

    /**
     * The port of a machine.
     */
    string port_;
};

#endif	/* _ADDRESSPORT_H */


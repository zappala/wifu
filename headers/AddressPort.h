/* 
 * File:   AddressPort.h
 * Author: rbuck
 *
 * Created on October 30, 2010, 10:06 AM
 */

#ifndef _ADDRESSPORT_H
#define	_ADDRESSPORT_H

#include <string>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sstream>
#include <iostream>
#include "exceptions/InvalidAddressException.h"

#include "GarbageCollector.h"

using namespace std;

/**
 * Represents an Address-Port combination.
 */
class AddressPort : public gc {
public:

    /**
     * Constructor: Stores address and port.
     *
     * @param address Address to store
     * @param port Port to store
     */
    AddressPort(string& address, uint16_t port);

    /**
     * Constructor: Stores address and port.
     *
     * @param address Address to store
     * @param port Port to store
     */
    AddressPort(const char* address, uint16_t port);

    /**
     * Constructor: Stores address and port inside obj.
     *
     * @param obj sockaddr_in pointer which contains an address and a port.
     */
    AddressPort(struct sockaddr_in* obj);

    /**
     * Copy Constructor
     * Copies the address, port, and internal struct to the new AddressPort object
     *
     * @param original The AddressPort from which to copy data out of
     */
    AddressPort(const AddressPort& original);

    /**
     * Assignment Operator
     * Copies the address, port, and internal struct to the new AddressPort object
     *
     * @param original The AddressPort from which to copy data out of
     */
    AddressPort & operator=(const AddressPort& original);

    /**
     * Operator equals
     *
     * @param other The "other" Address port to compare this one with
     * @return True if the addresses and the ports are equivialent, false otherwise
     */
    bool operator==(const AddressPort& other) const;

    /**
     * Operator not equals
     *
     * @param other The "other" Address port to compare this one with
     * @return True if either the addresses or ports do not match, false if addresses and ports are equivalent
     */
    bool operator!=(const AddressPort& other) const;

    /**
     * Destructor
     */
    virtual ~AddressPort();

    /**
     * @return A reference to the address
     */
    string& get_address();

    /**
     * @return A reference to the port
     */
    uint16_t get_port();

    /**
     * @return Reference to the data in struct format
     */
    struct sockaddr_in& get_network_struct();

    /**
     * @return Pointer to the data in struct format
     */
    struct sockaddr_in* get_network_struct_ptr();

    /**
     * Returns a string representation of this AddressPort
     *
     * @return A string representation of this AddressPort
     */
    string to_s();

private:
    /**
     * The (IP) address of a machine.
     */
    string address_;

    /**
     * The port of a machine.
     */
    uint16_t port_;

    /**
     * Address and port in struct format
     */
    struct sockaddr_in data_;

    /**
     * Helper Constructor: Stores address and port.
     *
     * @param address Address to store
     * @param port Port to store
     */
    void init(const char* address, uint16_t port);
};

/**
 * Comparator used to compare two AddressPort pointers.
 */
class AddressPortComparator {
public:

    /**
     * Compares two AddressPort pointers (actually references to AddressPort pointers).
     * Will return t1 < t2.
     *
     * @param ap1 The LHS of the expression ap1 < ap2
     * @param ap2 the RHS of the expression ap1 < ap2
     * @return True if t1 < t2, false otherwise.
     */
    bool operator()(AddressPort* const& ap1, AddressPort* const& ap2);
};

#endif	/* _ADDRESSPORT_H */


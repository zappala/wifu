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
    AddressPort(string& address, uint16_t port) : address_(address), port_(port) {
        init(address.c_str(), port);
    }

    /**
     * Constructor: Stores address and port.
     *
     * @param address Address to store
     * @param port Port to store
     */
    AddressPort(const char* address, uint16_t port) : address_(string(address)), port_(port) {
        init(address, port);
    }

    /**
     * Constructor: Stores address and port inside obj.
     *
     * @param obj sockaddr_in pointer which contains an address and a port.
     */
    AddressPort(struct sockaddr_in* obj) {

        assert(obj);

        char ip_address[INET_ADDRSTRLEN];
        u_int32_t saddr = obj->sin_addr.s_addr;
        inet_ntop(AF_INET, &saddr, ip_address, INET_ADDRSTRLEN);

        address_ = string(ip_address);
        port_ = ntohs(obj->sin_port);

        memcpy(&data_, obj, sizeof (struct sockaddr_in));
    }

    /**
     * Copy Constructor
     * Copies the address, port, and internal struct to the new AddressPort object
     *
     * @param original The AddressPort from which to copy data out of
     */
    AddressPort(const AddressPort& original) : address_(original.address_), port_(original.port_) {
        memcpy(&data_, &original.data_, sizeof (struct sockaddr_in));
    }

    /**
     * Assignment Operator
     * Copies the address, port, and internal struct to the new AddressPort object
     *
     * @param original The AddressPort from which to copy data out of
     */
    AddressPort & operator=(const AddressPort& original) {
        if (this != &original) {
            address_ = original.address_;
            port_ = original.port_;
            memcpy(&data_, &original.data_, sizeof (struct sockaddr_in));
        }
        return *this;
    }

    /**
     * Operator equals
     *
     * @param other The "other" Address port to compare this one with
     * @return True if the addresses and the ports are equivialent, false otherwise
     */
    bool operator==(const AddressPort& other) {
        return address_ == other.address_ && port_ == other.port_;
    }

    /**
     * Operator not equals
     *
     * @param other The "other" Address port to compare this one with
     * @return True if either the addresses or ports do not match, false if addresses and ports are equivalent
     */
    bool operator!=(const AddressPort& other) {
        return !operator ==(other);
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
    uint16_t get_port() {
        return port_;
    }

    /**
     * @return Reference to the data in struct format
     */
    struct sockaddr_in& get_network_struct() {
        return data_;
    }

    /**
     * @return Pointer to the data in struct format
     */
    struct sockaddr_in* get_network_struct_ptr() {
        return &data_;
    }

    /**
     * Returns a string representation of this AddressPort
     *
     * @return A string representation of this AddressPort
     */
    string to_s() {
        stringstream s;
        s << "Address: " << get_address() << " ";
        s << "Port: " << get_port();
        return s.str();
    }

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
    void init(const char* address, uint16_t port) {
        data_.sin_family = AF_INET;
        data_.sin_port = htons(port);

        if (!inet_aton(address, &data_.sin_addr)) {
            cout << "error converting ip address to binary" << endl;
            assert(false);
        }
    }
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
    bool operator()(AddressPort* const& ap1, AddressPort* const& ap2) {
        return ap1->to_s() < ap2->to_s();
    }
};

#endif	/* _ADDRESSPORT_H */


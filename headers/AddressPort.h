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
    AddressPort(string& address, int& port) : address_(address), port_(port) {
        data_.sin_family = AF_INET;
        data_.sin_port = htons(port);

        if (!inet_aton(address.c_str(), &data_.sin_addr)) {
            cout << "error converting ip address to binary" << endl;
            assert(false);
        }
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

    AddressPort(const AddressPort& original) : address_(original.address_), port_(original.port_) {
        memcpy(&data_, &original.data_, sizeof(struct sockaddr_in));
    }

    AddressPort& operator=(const AddressPort& original) {
        if(this != &original) {
            address_ = original.address_;
            port_ = original.port_;
            memcpy(&data_, &original.data_, sizeof(struct sockaddr_in));
        }
        return *this;
    }

    bool operator==(const AddressPort& original) {
        return address_ == original.address_ && port_ == original.port_;
    }

    bool operator!=(const AddressPort& original) {
        return !operator ==(original);
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
    int& get_port() {
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

private:
    /**
     * The (IP) address of a machine.
     */
    string address_;

    /**
     * The port of a machine.
     */
    int port_;

    /**
     * Address and port in struct format
     */
    struct sockaddr_in data_;



};

#endif	/* _ADDRESSPORT_H */


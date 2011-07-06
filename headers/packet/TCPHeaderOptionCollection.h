/* 
 * File:   TCPHeaderOptionCollection.h
 * Author: rbuck
 *
 * Created on April 28, 2011, 10:31 AM
 */

#ifndef TCPHEADEROPTIONCOLLECTION_H
#define	TCPHEADEROPTIONCOLLECTION_H

#include <list>
#include <sys/types.h>
#include "TCPHeaderOption.h"
#include "visitors/Visitable.h"
#include "visitors/FindTCPHeaderOptionVisitor.h"
#include "visitors/GetTCPHeaderOptionsLengthVisitor.h"

using namespace std;

/**
 * Collection of TCPHeaderOptions
 *
 * @see Visitable
 */
class TCPHeaderOptionCollection : public Visitable {
public:
    /**
     * Constructor.
     */
    TCPHeaderOptionCollection();

    /**
     * Destructor.
     */
    virtual ~TCPHeaderOptionCollection();

    /**
     * Inserts option into the ollection.
     * @param option The TCPHeaderOption to insert.
     */
    void insert(TCPHeaderOption* option);

    /**
     * Removes the TCPHeaderOption denoted by kind from the collection.
     * @param kind The kind of TCPHeaderOption to remove.
     * @return The removed TCPHeaderOption if it was found, NULL otherwise.
     */
    TCPHeaderOption* remove(u_int8_t kind);

    /**
     * @param kind The kind of TCP header to search for.
     * @return True, if this collection contains a TCPHeaderOption with kind kind.
     */
    bool contains(u_int8_t kind);

    /**
     * Function that allows Visitor objects to visit it.
     * Iterates over each item in this collection calling Visitor::visit() on it.
     * @param v The Visitor that is visiting.
     *
     * @see Visitor
     * @see Visitable
     */
    void accept(Visitor* v);

    /**
     * @return True if there is at least one TCPHeaderOption in this collection, false otherwise.
     */
    bool empty();

    /**
     * Parses a buffer containing TCP options.
     * Handles the NOP and the end of options bytes.
     * This is to be used when we receive a packet from the network and need to parse all the options.
     *
     * @post All options that are in options are now in TCPHeaderOption objects.
     *
     * @param options Pointer to buffer containing TCP header options.
     * @param options_length_bytes Length of the options in bytes.
     */
    void parse(unsigned char* options, u_int8_t options_length_bytes);
    

private:

    /**
     * Container to store the TCPHeaderOptions.
     */
    list<TCPHeaderOption*> options_;
};

#endif	/* TCPHEADEROPTIONCOLLECTION_H */


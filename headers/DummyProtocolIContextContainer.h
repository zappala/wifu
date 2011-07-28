/* 
 * File:   DummyProtocolIContextContainer.h
 * Author: rbuck
 *
 * Created on July 19, 2011, 12:02 PM
 */

#ifndef DUMMYPROTOCOLICONTEXTCONTAINER_H
#define	DUMMYPROTOCOLICONTEXTCONTAINER_H

#include "contexts/TCPTahoeIContextContainer.h"


class DummyProtocolIContextContainer : public TCPTahoeIContextContainer {
public:
    DummyProtocolIContextContainer();
    
    ~DummyProtocolIContextContainer();
    
    ReceiveEvent* get_receive_event() const;

    void set_receive_event(ReceiveEvent* e);

    int get_receive_index() const;

    void set_receive_index(int receive_index);

private:
    ReceiveEvent* receive_event_;

    int receive_index_;
};

#endif	/* DUMMYPROTOCOLICONTEXTCONTAINER_H */


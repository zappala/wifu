/* 
 * File:   DataEvent.h
 * Author: rbuck
 *
 * Created on August 11, 2011, 10:16 AM
 */

#ifndef DATAEVENT_H
#define	DATAEVENT_H

#include "LibraryEvent.h"

class DataEvent : public LibraryEvent {
public:
    DataEvent();
    virtual ~DataEvent();

    size_t get_data_length();
    int get_flags();
};


#endif	/* DATAEVENT_H */


#include "events/framework_events/DataEvent.h"

DataEvent::DataEvent() : LibraryEvent() {

}

DataEvent::~DataEvent() {

}

size_t DataEvent::get_data_length() {
    return ((struct DataMessage*) get_buffer())->buffer_length;
}

int DataEvent::get_flags() {
    return ((struct DataMessage*) get_buffer())->flags;
}
